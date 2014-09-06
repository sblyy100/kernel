#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
int main(){
	int sock,sock_rc;
        int tos = 16;
        sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
        struct sockaddr_in recv_addr;
	struct msghdr out_message;
	struct iovec iovectors[1];
	unsigned char control_info[CMSG_SPACE(sizeof tos)];
	struct cmsghdr *cmsg;

        recv_addr.sin_family = AF_INET;
        recv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        recv_addr.sin_port = htons(5000);
 	out_message.msg_name = &recv_addr;
	out_message.msg_namelen = sizeof(recv_addr);
	out_message.msg_iov = iovectors;
	out_message.msg_iovlen = 1;
	out_message.msg_control = control_info;
	out_message.msg_controllen = sizeof(control_info);
	out_message.msg_flags = 0;
	iovectors[0].iov_base = "hello";
	iovectors[0].iov_len = strlen("hello");

	cmsg = CMSG_FIRSTHDR(&out_message);
	cmsg->cmsg_len = CMSG_LEN(sizeof(int));
	cmsg->cmsg_level = SOL_IP;
	cmsg->cmsg_type = IP_TOS;
	*(int*)CMSG_DATA(cmsg) = tos; /* actual DSCP value to be used */

	sock_rc = sendmsg (sock, &out_message, 0);
}

