#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/netdevice.h>
#include <linux/ip.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SBL");
MODULE_DESCRIPTION("netfilter test");
#define NF_IP_PRE_ROUTING 0
/* 用于注册我们的函数的数据结构 */
#define NIPQUAD( _addr_ ) \
	 ((const unsigned char *)&( _addr_ ) )[0], \
 ((const unsigned char *)&( _addr_ ) )[1], \
 ((const unsigned char *)&( _addr_ ) )[2], \
 ((const unsigned char *)&( _addr_ ) )[3]     //!< 以可读格式显示 IP 地址，参数为网络字节序
 #define NIPQUAD_FMT "%u.%u.%u.%u"     //!< IP 地址显示格式
static struct nf_hook_ops nfho[5];

/* 注册的hook函数的实现 */
unsigned int hook_func(unsigned int hooknum,
                       struct sk_buff *skb,
                       const struct net_device *in,
                       const struct net_device *out,
                       int (*okfn)(struct sk_buff *))
{
	unsigned short p;
	struct iphdr *ih;
	if(skb && skb->mark){
		p = ntohs(skb->protocol);
		printk("mask:%u\n",skb->mark);
		if(p == 0x800){
			ih = ip_hdr(skb);
			printk("%u.%u.%u.%u<-->%u.%u.%u.%u\n",NIPQUAD(ih->saddr),NIPQUAD(ih->daddr));
			//ih->daddr = htonl(INADDR_LOOPBACK);
			//printk("skb type:%d\n",skb->pkt_type);
			//skb->pkt_type = PACKET_HOST;
		}
	}
	return NF_ACCEPT;
}
unsigned int hook_func_in(unsigned int hooknum,
                       struct sk_buff *skb,
                       const struct net_device *in,
                       const struct net_device *out,
                       int (*okfn)(struct sk_buff *))
{
	unsigned short p;
	struct iphdr *ih;
	if(skb && skb->mark){
		p = ntohs(skb->protocol);
		printk("local in mask:%u\n",skb->mark);
		if(p == 0x800){
			ih = ip_hdr(skb);
			//printk("%u.%u.%u.%u<-->%u.%u.%u.%u\n",NIPQUAD(ih->saddr),NIPQUAD(ih->daddr));
		}
	}
	return NF_ACCEPT;
}
unsigned int hook_func_forward(unsigned int hooknum,
                       struct sk_buff *skb,
                       const struct net_device *in,
                       const struct net_device *out,
                       int (*okfn)(struct sk_buff *))
{
	unsigned short p;
	struct iphdr *ih;
	if(skb && skb->mark){
		p = ntohs(skb->protocol);
		printk("forward in mask:%u\n",skb->mark);
		if(p == 0x800){
			ih = ip_hdr(skb);
			//printk("%u.%u.%u.%u<-->%u.%u.%u.%u\n",NIPQUAD(ih->saddr),NIPQUAD(ih->daddr));
		}
	}
	return NF_ACCEPT;
}




/* 初始化程序 */
int hello_init(void)
{
	nfho[0].hook = hook_func;         /* 处理函数 */
	nfho[0].hooknum  = NF_IP_PRE_ROUTING; /* 使用IPv4的第一个hook */
	nfho[0].pf       = PF_INET;
	//nfho.priority = NF_IP_PRI_FIRST;   /* 让我们的函数首先执行 */	
	nfho[0].priority = 300;   /* 让我们的函数首先执行 */

	nfho[1].hook = hook_func_in;         /* 处理函数 */
	nfho[1].hooknum  = NF_INET_LOCAL_IN; /* 使用IPv4的第一个hook */
	nfho[1].pf       = PF_INET;
	//nfho.priority = NF_IP_PRI_FIRST;   /* 让我们的函数首先执行 */	
	nfho[1].priority = NF_IP_PRI_FIRST;   /* 让我们的函数首先执行 */
    
	nfho[2].hook = hook_func_forward;         /* 处理函数 */
	nfho[2].hooknum  = NF_INET_FORWARD; /* 使用IPv4的第一个hook */
	nfho[2].pf       = PF_INET;
	//nfho.priority = NF_IP_PRI_FIRST;   /* 让我们的函数首先执行 */	
	nfho[2].priority = NF_IP_PRI_FIRST;   /* 让我们的函数首先执行 */
	nf_register_hook(&nfho[0]);
	nf_register_hook(&nfho[1]);
	nf_register_hook(&nfho[2]);
	return 0;
}

/* 清除程序 */
void hello_exit(void)
{
    nf_unregister_hook(&nfho[0]);
    nf_unregister_hook(&nfho[1]);
    nf_unregister_hook(&nfho[2]);
}
module_init(hello_init);
module_exit(hello_exit);
