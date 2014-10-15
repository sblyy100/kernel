#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include "my_kernel.h"
struct rule * match_rule(struct list_head *rh,struct sk_buff *skb,struct net_device *in,struct net_device *out){
	return NULL;
}
int do_action(struct rule *r,struct sk_buff *skb){
	return NF_ACCEPT;
}
EXPORT_SYMBOL(match_rule);
EXPORT_SYMBOL(do_action);
