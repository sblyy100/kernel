#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/sctp.h>
#include <netdb.h>
#include <string.h>

#define die(x)			\
	do {			\
		perror((x));	\
		exit(-1);	\
	} while(0)

int main(int argc, char *argv[])
{
	int sk;
	int flags = 0;
	struct addrinfo *res;
	union {
		struct sockaddr_in6 addr6;
		struct sockaddr addr;
	} ser_addr;
	if (getaddrinfo(argv[1], argv[2], NULL, &res) !=0) {
		fprintf(stderr, "getaddrinfo: address wrong!\n");
		exit(-1);
	}
	memcpy(&ser_addr, res->ai_addr, res->ai_addrlen);
	struct iovec iov={"hello",5};
	struct msghdr msg={
		.msg_name = &ser_addr,
		.msg_namelen = sizeof(ser_addr),
		.msg_iov = &iov,
		.msg_iovlen = 1,
	};

	if ((sk = socket(res->ai_family, SOCK_SEQPACKET, IPPROTO_SCTP)) == -1)
		die("socket");

	sendmsg(sk,&msg,0);

	free(res);
	close(sk);
	return 0;
}
