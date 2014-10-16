#ifndef _HANDLE_H
#define _HANDLE_H
#include "main.h"
extern struct rule * match_rule(struct list_head *rh,struct sk_buff *skb,const struct net_device *in,const struct net_device *out);
extern int do_action(struct rule *r,struct sk_buff *skb);

#endif
