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
struct nl_info *g_req=NULL;
static void usage(int argc,char **argv){
	char *app = argv[0];
	printf("%s:\n",argv[0]);
	printf("-a\t add rule\n");
	printf("-d\t delete rule:\n");
	printf("-m\t modity rule:\n");
	exit(-1);
}
static void dump_rule(struct rule *r){
	printf("rule-id:%u\n",r->id);
	printf("rule-protocol:%u\n",r->protocol);
	printf("rule-src:%u\n",r->src);
	printf("rule-dst:%u\n",r->dst);
	printf("rule-sport:%u\n",r->sport);
	printf("rule-dport:%u\n",r->dport);
	printf("rule-action:%u\n",r->action);
}
static int init_req(struct nl_info *req){
	int len = sizeof(struct nl_info)+sizeof(struct rule);
	g_req = (struct nl_info*)malloc(len);
	if(!g_req){
		perror("malloc for nl_info failed");
		return -1;
	}
	memset(g_req, 0, len);
	return 0;
}
static void free_req(struct nl_info *req){
	if(req)
		free(req);
}
static int fill_msg(struct nl_info *req,int argc,char **argv){
	int opt;
	struct rule *r;
	//req->nh.nlmsg_len = NLMSG_LENGTH(sizeof(struct genlmsghdr)+sizeof(struct my_msg));
	req->nh.nlmsg_len = NLMSG_LENGTH(sizeof(struct genlmsghdr)+sizeof(struct my_msg)+sizeof(struct rule));
	req->nh.nlmsg_flags = NLM_F_REQUEST;
	req->nh.nlmsg_type = GENL_ID_TEST;

	req->genlhdr.cmd = 0x1;
	req->genlhdr.version = 0x1;
	req->uh.cmd = RULE_NONE;
	r = (struct rule *)req->uh.r;
	printf("%dvs%dvs%d\n",req->nh.nlmsg_len,sizeof(struct genlmsghdr),sizeof(struct my_msg));
	while ((opt = getopt(argc, argv, "A:D:M:i:o:d:s:")) != -1) {
		switch(opt){
			case 'A':
				req->uh.cmd = RULE_ADD;
				r->id = atoi(optarg);
				break;
			case 'D':
				req->uh.cmd = RULE_DEL;
				r->id = atoi(optarg);
				break;
			case 'M':
				req->uh.cmd = RULE_MOD;
				r->id = atoi(optarg);
				break;
			case 'i':
				strncpy(r->from,optarg,16);
				break;
			case 'o':
				strncpy(r->to,optarg,16);
				break;
			case 'd':
				r->dport = htons(atoi(optarg) & 0xffff);
				break;
			case 's':
				r->sport = htons(atoi(optarg) & 0xffff);
				break;
			default:
				req->uh.cmd = RULE_NONE;
				break;
		}
	}
	if(req->uh.cmd == RULE_NONE)
		return -1;
	return 0;
}
int main(int argc,char **argv)
{
	int nlfd;
	int opt;
	struct sockaddr_nl sock_loc, sock_ker;
	init_req(g_req);
	if(fill_msg(g_req,argc,argv)<0){
		usage(argc,argv);
	}
	if(g_req)
		printf("cmd:%d\n",g_req->uh.cmd);
	dump_rule(&g_req->uh.r);
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

        if (send(nlfd, g_req, g_req->nh.nlmsg_len, 0) == -1) {
                perror("send message to kernel error");
                exit(EXIT_FAILURE);
        }
		free_req(g_req);
        return 0;
}
