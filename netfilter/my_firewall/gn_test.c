#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <asm/types.h>
#include <netinet/in.h>
#include <linux/socket.h>
#include <linux/netlink.h>
#include <linux/genetlink.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include "my_msg.h"
#define GENL_ID_TEST 0x110
int main()
{
        int nlfd;
        struct sockaddr_nl sock_loc, sock_ker;

        struct {
                struct nlmsghdr nh;
                struct genlmsghdr genlhdr;
				struct my_msg my;
        } req;

        memset(&req, 0, sizeof(req));
        req.nh.nlmsg_len = NLMSG_LENGTH(sizeof(struct genlmsghdr)+sizeof(struct my_msg));
        req.nh.nlmsg_flags = NLM_F_REQUEST;
        req.nh.nlmsg_type = GENL_ID_TEST;

        req.genlhdr.cmd = 0x1;
        req.genlhdr.version = 0x1;

		req.my.cmd = RULE_ADD;
		req.my.count= 1;
		//req.my.data= NULL;

        if ((nlfd = socket(AF_NETLINK, SOCK_RAW, NETLINK_GENERIC)) == -1) {
                perror("netlink socket create failed");
                exit(EXIT_FAILURE);
        }

        memset(&sock_loc, 0, sizeof(sock_loc));
        sock_loc.nl_family = AF_NETLINK;
        sock_loc.nl_pid = getpid();
        if (bind(nlfd, (struct sockaddr *)&sock_loc, sizeof(sock_loc)) == -1) {
                perror("bind local netlink socket failed");
                exit(EXIT_FAILURE);
        }

        memset(&sock_ker, 0, sizeof(sock_ker));
        sock_ker.nl_family = AF_NETLINK;
        sock_ker.nl_pid = 0;
        if (connect(nlfd, (struct sockaddr *) &sock_ker, sizeof(sock_ker)) == -1) {
                perror("netlink socket connect kernel failed ");
                exit(EXIT_FAILURE);
        }

        if (send(nlfd, &req, req.nh.nlmsg_len, 0) == -1) {
                perror("send message to kernel error");
                exit(EXIT_FAILURE);
        }
        return 0;
}
