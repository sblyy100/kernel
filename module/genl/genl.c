#include <net/genetlink.h>
#include <linux/module.h>
#include "my_msg.h"
#define TEST_GENL_NAME "GENLTEST"
#define TEST_GENL_VERSION 0x1
#define TEST_GENL_HDRLEN 0
#define GENL_ID_TEST 0x110

#define TEST_GENL_CMD 0x1
/*
 66 struct genl_info
  67 {
   68 |   u32||   |   snd_seq;
    69 |   u32||   |   snd_pid;
	 70 |   struct nlmsghdr *|  nlhdr;
	  71 |   struct genlmsghdr *|genlhdr;
	   72 |   void *| |   |   userhdr;
	    73 |   struct nlattr **|   attrs;
		 74 #ifdef CONFIG_NET_NS
		  75 |   struct net *|   |   _net;
		   76 #endif
		    77 };

 * */
static int handle_test_cmd(struct sk_buff *skb, struct genl_info *info)
{
		struct my_msg *my=NULL;
		if(info->userhdr)
			my = (struct my_msg*)info->userhdr;
        printk("[handle_test_cmd]start==>");
		printk("snd_seq:%u\n",info->snd_seq);
		printk("snd_pid:%u\n",info->snd_pid);
		printk("nlhdr->nlmsg_len:%u\n",info->nlhdr->nlmsg_len);
		printk("nlhdr->nlmsg_flags:%u\n",info->nlhdr->nlmsg_flags);
		printk("nlhdr->nlmsg_type:%u\n",info->nlhdr->nlmsg_type);
		printk("genlhdr->cmd:%u\n",info->genlhdr->cmd);
		printk("genlhdr->version:%u\n",info->genlhdr->version);
		if(my){
			printk("my->cmd:%u\n",my->cmd);
			printk("my->value:%u\n",my->value);
		}
        msleep(10000);
        printk("[handle_test_cmd]end\n");
        //mdelay(200);
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
        .doit           = handle_test_cmd,
};

static int family_registered = 1;

static int __init genltest_init(void)
{
        if (genl_register_family(&family))
                goto err;

        family_registered = 1;

        if (genl_register_ops(&family, &ops))
                goto err_unregister;

        return 0;

err_unregister:
        genl_unregister_family(&family);
        family_registered = 0;
err:
        printk("Failed to register genltest interface\n");
        return -EFAULT;
}

static void __exit genltest_exit(void)
{
        if (family_registered) {
                genl_unregister_family(&family);
                family_registered = 0;
        }
}

module_init(genltest_init);
module_exit(genltest_exit);
MODULE_LICENSE("GPL");
