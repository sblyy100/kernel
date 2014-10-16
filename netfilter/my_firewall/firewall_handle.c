#include "my_kernel.h"
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SBL");
MODULE_DESCRIPTION("netfilter test");
struct rule * match_rule(struct list_head *rh,struct sk_buff *skb,const struct net_device *in,const struct net_device *out){
	return NULL;
}
int do_action(struct rule *r,struct sk_buff *skb){
	return NF_ACCEPT;
}
//EXPORT_SYMBOL(match_rule);
//EXPORT_SYMBOL(do_action);
