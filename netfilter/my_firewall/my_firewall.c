#include "my_kernel.h"
#include "firewall_handle.h"
#include "main.h"
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SBL");
MODULE_DESCRIPTION("netfilter test");
struct list_head r_set;
static void init_rule(struct list_head *rh){
	INIT_LIST_HEAD(rh);
}
static void fini_rule(struct list_head *rh){
	struct rule *r,*n;
	list_for_each_entry_safe(r,n,rh,list){
		list_del(&r->list);
		kfree(r);
	}
}
static void dump_rule(struct rule *r){
	printk("rule-id:%u\n",r->id);
	printk("rule-protocol:%u\n",r->protocol);
	printk("rule-src:%u\n",r->src);
	printk("rule-dst:%u\n",r->dst);
	printk("rule-sport:%u\n",r->sport);
	printk("rule-dport:%u\n",r->dport);
	printk("rule-action:%u\n",r->action);
}

static int handle_rule(struct rule *r){
	int ret;
	if(!r){
		printk("handle rule empty!\n");
		return -1;
	}
	dump_rule(r);
	return 0;

}

static int handle_firewall_cmd(struct sk_buff *skb, struct genl_info *info)
{
		struct my_msg *my=NULL;
		struct rule *r;
        printk(KERN_ERR"[handle_test_cmd]start==>");
		printk("snd_seq:%u\n",info->snd_seq);
		printk("snd_pid:%u\n",info->snd_pid);
		printk("nlhdr->nlmsg_len:%u\n",info->nlhdr->nlmsg_len);
		printk("nlhdr->nlmsg_flags:%u\n",info->nlhdr->nlmsg_flags);
		printk("nlhdr->nlmsg_type:%u\n",info->nlhdr->nlmsg_type);
		printk("genlhdr->cmd:%u\n",info->genlhdr->cmd);
		printk("genlhdr->version:%u\n",info->genlhdr->version);
		if(info->userhdr)
			my = (struct my_msg*)info->userhdr;
		if(my){
			printk("my->cmd:%u\n",my->cmd);
			r = (struct rule*)my->data;
			if(r){
				//handle_rule(r);
			}
		}
	return 0;
}

static struct genl_family family = {
        .id             = GENL_ID_TEST,
        .name           = TEST_GENL_NAME,
        .version        = TEST_GENL_VERSION,
        .hdrsize        = TEST_GENL_HDRLEN,
        .maxattr        = 0,
};
static struct genl_ops ops = {
        .cmd            = TEST_GENL_CMD,
        .doit           = handle_firewall_cmd,
};

unsigned int firewall_func(unsigned int hooknum,
                       struct sk_buff *skb,
                       const struct net_device *in,
                       const struct net_device *out,
                       int (*okfn)(struct sk_buff *))
{
	struct rule *r;
    r = match_rule(&r_set,skb,in,out);
	if(r){
		do_action(r,skb);
	}
	return NF_ACCEPT;
}

static struct nf_hook_ops nfho={
    .hook = firewall_func,
    .hooknum  = NF_INET_PRE_ROUTING,
    .pf       = PF_INET,
    .priority = NF_IP_PRI_FIRST,
};


static int  genl_firewall_init(void)
{
	if (genl_register_family(&family))
		goto err;

	if (genl_register_ops(&family, &ops))
		goto err_unregister;
	printk(KERN_ERR"success to register genltest interface\n");

	return 0;

err_unregister:
	genl_unregister_family(&family);
err:
	printk("Failed to register genltest interface\n");
	return -EFAULT;
}
static void genl_firewall_exit(void)
{
	genl_unregister_family(&family);
}
static int do_hook(void){
    nf_register_hook(&nfho);
	return 0;
}
static void undo_hook(void){
    nf_unregister_hook(&nfho);
}
/* 初始化程序 */
int __init firewall_init(void)
{
	init_rule(&r_set);
	genl_firewall_init();
	do_hook();
	return 0;
}

/* 清除程序 */
void __exit firewall_exit(void)
{
	undo_hook();
	genl_firewall_exit();
	fini_rule(&r_set);
	printk(KERN_ERR"firewall exit\n");
}
module_init(firewall_init);
module_exit(firewall_exit);
