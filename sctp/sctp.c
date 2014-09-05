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
static void usage(char *app){
	printf("usage:\n");
	printf("%s rto_initial: get value of rto_initial\n",app);
	printf("%s rto_max: get value of rto_max\n",app);
	printf("%s rto_min: get value of rto_min\n",app);
	printf("%s valid_cookie_life: get value of valid_cookie_life\n",app);
	printf("\n");
}
struct sysctl_ops {
	int	sock;
	char	*name;
	int	optname;
	int	level;
	void	*data;
	int	len;
	int	(*get_value)(struct sysctl_ops*);
	
};
static int get_rto_initial(struct sysctl_ops *ops){
	int ret = -1;

	if(ops->sock < 0){
		return -1;
	}
	ret = getsockopt(ops->sock,ops->level,ops->optname,ops->data,&ops->len);
	if (ret < 0){
		return ret;
	}
	ret = ((struct sctp_rtoinfo*)ops->data)->srto_initial;
		return ret;
}
static int get_rto_max(struct sysctl_ops *ops){
	int ret = -1;

	if(ops->sock < 0){
		return -1;
	}
	ret = getsockopt(ops->sock,ops->level,ops->optname,ops->data,&ops->len);
	if (ret < 0){
		return ret;
	}
	ret = ((struct sctp_rtoinfo*)ops->data)->srto_max;
		return ret;
}
static int get_rto_min(struct sysctl_ops *ops){
	int ret = -1;

	if(ops->sock < 0){
		return -1;
	}
	ret = getsockopt(ops->sock,ops->level,ops->optname,ops->data,&ops->len);
	if (ret < 0){
		perror("min");
		return ret;
	}
	ret = ((struct sctp_rtoinfo*)ops->data)->srto_min;
		return ret;
}
static int get_cookie_life(struct sysctl_ops *ops){
	int ret = -1;

	if(ops->sock < 0){
		return -1;
	}
	ret = getsockopt(ops->sock,ops->level,ops->optname,ops->data,&ops->len);
	if (ret < 0){
		perror("cookie");
		return ret;
	}
	ret = ((struct sctp_assocparams*)ops->data)->sasoc_cookie_life;
		return ret;
}
static int print_ops(struct sysctl_ops *ops){
	printf("=====================\n");
	printf("%u\n",ops->level);
	printf("%u\n",ops->optname);
	printf("%s\n",ops->name);
	printf("=====================\n");
}

//typedef int(*get_value)(int sock,int int level, int optname,void *optval, socklen_t *optlen)

struct sctp_rtoinfo rto;
struct sctp_assocparams asps;
static struct sysctl_ops test_ops[]={
	{
		.sock = -1,
		.name = "rto_initial",
		.level = SOL_SCTP,
		.optname = SCTP_RTOINFO,
		.data = &rto,
		.len = sizeof(rto),
		.get_value = get_rto_initial,
	},
	{
		.sock = -1,
		.name = "rto_max",
		.level = SOL_SCTP,
		.optname = SCTP_RTOINFO,
		.data = &rto,
		.len = sizeof(rto),
		.get_value = get_rto_max,
	},
	{
		.sock = -1,
		.name = "rto_min",
		.level = SOL_SCTP,
		.optname = SCTP_RTOINFO,
		.data = &rto,
		.len = sizeof(rto),
		.get_value = get_rto_min,
	},
	{
		.sock = -1,
		.name = "valid_cookie_life",
		.level = SOL_SCTP,
		.optname = SCTP_ASSOCINFO,
		.data = &asps,
		.len = sizeof(asps),
		.get_value = get_cookie_life,
	},
	{NULL,NULL,},
};
int main(int argc, char *argv[])
{

	int sk = -1;
	struct sysctl_ops *t;
	int i;
	int value = -1;
	if(argc != 2){
		usage(argv[0]);
		exit(-1);
	}
	for(i=0;t = &test_ops[i],t->sock!=NULL;i++){
		if(strcmp(argv[1],t->name) == 0){
			if ((sk = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP)) == -1)
				die("socket");
			t->sock = sk;
			//print_ops(t);
			value = t->get_value(t);
		}
	}
	close(sk);
	printf("%d\n",value);
}

