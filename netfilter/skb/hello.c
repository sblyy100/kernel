#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/netdevice.h>
#include <linux/ip.h>
//#include <uapi/linux/netfilter_ipv4.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SBL");
MODULE_DESCRIPTION("netfilter test");
#define NF_IP_PRE_ROUTING 0
/* 用于注册我们的函数的数据结构 */
static struct nf_hook_ops nfho;
static int print_skb(struct sk_buff *skb){
	unsigned short p;
	struct iphdr *ih;
	p = ntohs(skb->protocol);
	//printk(KERN_INFO"skb protocol:%u\n",p);
	if(skb && p==0x0800){
		ih = ip_hdr(skb);
		//printk(KERN_ERR"protocol:%u\n",ih->protocol);
		if(ih->protocol == 1){
			//icmp
		printk(KERN_ERR"==========ip packet============\n");
		if(skb->mac_header)
			printk(KERN_ERR"mac_header:%p\n",skb->mac_header);
		if(skb->network_header)
			printk(KERN_ERR"network_header:%p\n",skb->network_header);
		if(skb->transport_header)
			printk(KERN_ERR"transport_header:%p\n",skb->transport_header);
		if(skb->tail)
			printk(KERN_ERR"tail:%p\n",skb->tail);
		if(skb->end)
			printk(KERN_ERR"end:%p\n",skb->end);
		if(skb->head)
			printk(KERN_ERR"head:%u\n",(unsigned int)skb->head);
		if(skb->data)
			printk(KERN_ERR"data:%u\n",(unsigned int)skb->data);
		printk(KERN_ERR"len:%u,data len:%u\n",skb->len,skb->data_len);
		// head data is pointor; tail/end/xx_header is offset
		//skb_push(skb,14); data- up len+   encode
		skb_pull(skb,14);// data+ down len- decode

		
		printk(KERN_ERR"==========ip packet after============\n");
		if(skb->mac_header)
			printk(KERN_ERR"mac_header:%p\n",skb->mac_header);
		if(skb->network_header)
			printk(KERN_ERR"network_header:%p\n",skb->network_header);
		if(skb->transport_header)
			printk(KERN_ERR"transport_header:%p\n",skb->transport_header);
		if(skb->tail)
			printk(KERN_ERR"tail:%p\n",skb->tail);
		if(skb->end)
			printk(KERN_ERR"end:%p\n",skb->end);
		if(skb->head)
			printk(KERN_ERR"head:%u\n",(unsigned int)skb->head);
		if(skb->data)
			printk(KERN_ERR"data:%u\n",(unsigned int)skb->data);
		printk(KERN_ERR"len:%u,data len:%u\n",skb->len,skb->data_len);
		
		return NF_DROP;
			
		}

	}
}
/* 注册的hook函数的实现 */
unsigned int hook_func(unsigned int hooknum,
                       struct sk_buff *skb,
                       const struct net_device *in,
                       const struct net_device *out,
                       int (*okfn)(struct sk_buff *))
{
	int ret;
	if(!in)
		return NF_ACCEPT;
	if((strcmp(in->name,"lo"))==0){
		//printk(KERN_INFO"skb from lo\n");
		ret = print_skb(skb);
		return ret;
	}
    return NF_ACCEPT;           /* 丢弃所有的数据包 */
}

/* 初始化程序 */
int hello_init(void)
{
    /* 填充我们的hook数据结构 */
    nfho.hook = hook_func;         /* 处理函数 */
    nfho.hooknum  = NF_IP_PRE_ROUTING; /* 使用IPv4的第一个hook */
    nfho.pf       = PF_INET;
    nfho.priority = NF_IP_PRI_FIRST;   /* 让我们的函数首先执行 */

    nf_register_hook(&nfho);
	return 0;
}

/* 清除程序 */
void hello_exit(void)
{
    nf_unregister_hook(&nfho);
}
module_init(hello_init);
module_exit(hello_exit);
