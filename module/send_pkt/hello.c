#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/netfilter.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/inet.h>
#include <linux/netdevice.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/udp.h>
#include <net/udp.h>
#include <net/route.h>
#include <net/icmp.h>
#include <linux/netfilter_ipv4.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LUCIEN");

#define ETH "eth0"
#define SIP "10.66.12.136"
#define DIP "10.66.65.92"
#define SPORT 8000
#define DPORT 8000

unsigned char SMAC[ETH_ALEN] = {0x28,0xd2,0x44,0xc4,0x7c,0x2d};//28:d2:44:c4:7c:2d
//unsigned char DMAC[ETH_ALEN] = {0x90,0xE2,0xBA,0x4A,0x64,0xC0};
unsigned char DMAC[ETH_ALEN] = {0x52,0x54,0x00,0x12,0x34,0x56};//52:54:00:12:34:56


unsigned char pkt[500]="123";

int cp_dev_xmit_udp (char * eth, u_char * smac, u_char * dmac,
                u_char * pkt, int pkt_len,
                u_long sip, u_long dip,
                u_short sport, u_short dport, u_long seq, u_long ack_seq, u_char psh, u_char fin)
{
        struct sk_buff * skb = NULL;
        struct net_device * dev = NULL;
        struct ethhdr * ethdr = NULL;
        struct iphdr * iph = NULL;
        struct udphdr * udph = NULL;
        u_char * pdata = NULL;
        int nret = 1;

        if (NULL == smac || NULL == dmac) 
            goto out;

        dev = dev_get_by_name(&init_net, eth);
        if (NULL == dev)
            goto out;
        skb = alloc_skb (pkt_len + sizeof (struct iphdr) + sizeof (struct udphdr) + LL_RESERVED_SPACE (dev), GFP_ATOMIC);

        if (NULL == skb)
                goto out;
        skb_reserve (skb, LL_RESERVED_SPACE (dev));//add data and tail
        skb->dev = dev;
        skb->pkt_type = PACKET_OTHERHOST;
        skb->protocol = __constant_htons(ETH_P_IP);
        skb->ip_summed = CHECKSUM_NONE;
        skb->priority = 0;

        skb_set_network_header(skb, 0); //skb->network_header = skb->data + 0;
        skb_put(skb, sizeof (struct iphdr)); //add tail and len

        skb_set_transport_header(skb, sizeof (struct iphdr));//skb->transport_header = skb->data + sizeof (struct iphdr)
        skb_put(skb, sizeof (struct udphdr));

        pdata = skb_put (skb, pkt_len);
        {
                if (NULL != pkt)
                        memcpy (pdata, pkt, pkt_len);
        }

        {
                udph = udp_hdr(skb);
                memset (udph, 0, sizeof (struct udphdr));
                udph->source = sport;
                udph->dest = dport;
                udph->len = htons(pkt_len+sizeof(struct udphdr)); //remember htons
                udph->check = 0;
        }

        {
                iph = ip_hdr(skb);
                iph->version = 4;
                iph->ihl = sizeof(struct iphdr)>>2;
                iph->frag_off = 0;
                iph->protocol = IPPROTO_UDP;
                iph->tos = 0;
                iph->daddr = dip;
                iph->saddr = sip;
                iph->ttl = 0x40;
                iph->tot_len = __constant_htons(skb->len);
                iph->check = 0;//remember to set 0
                iph->check = ip_fast_csum((void *)iph,iph->ihl);
        }

        {
                ethdr = (struct ethhdr*)skb_push (skb, 14);//reduce data and add len
                memcpy (ethdr->h_dest, dmac, ETH_ALEN);
                memcpy (ethdr->h_source, smac, ETH_ALEN);
                ethdr->h_proto = __constant_htons (ETH_P_IP);
        }
                printk("%d\n", udph->len);
        if (0 > dev_queue_xmit(skb)) goto out;
        nret = 0;
out:
        if (0 != nret && NULL != skb)
        {
                dev_put (dev);
                kfree_skb (skb);
        }
        return (nret);
}

static int __init init(void)
{
        cp_dev_xmit_udp (ETH, SMAC, DMAC,pkt, sizeof(pkt),
                        in_aton(SIP),in_aton(DIP),
                        htons(SPORT),htons(DPORT),
                        1, 0, 0, 0);
        return 0;
}

static void __exit fini(void)
{
}

module_init(init);
module_exit(fini);

