#include "myhead.h"
#include "server.h"
#include "log.h"
#define MAX_WORKER 1

static void mydaemon(void) {
	//int fd0,fd1,fd2;
	if (0 != fork()) exit(0);//fork使子进程继承父进程的进程组，但是新的pid

	if (-1 == setsid()) exit(0);//设置回话id，使其成为新会话的首进程，进程组组长，失去控制终端
	signal(SIGHUP, SIG_IGN);

	if (0 != fork()) exit(0);//有 则pgid=sid,但是pid不等。  没有这句话，则pid=pgid=sid
	//为了防止进程再次获得控制终端，也可以只fork一次

	if (0 != chdir("/")) exit(0);
	/*
	fd0=open("/dev/null",O_RDWR);
	fd1=dup(0);
	fd2=dup(0);
	*/
}
main(int argc,char **argv){
	
	int child=0;
	int child_num=MAX_WORKER;
	//mylog("service begin",2,__FILE__,__LINE__);	
	//mydaemon();
	
	/*parent process:watch*/
	while(!child){
		if(child_num>0){
			switch(fork()){
				case -1:
					return -1;
				case 0:
					child=1;
					break;
				default:
					child_num--;
					break;
			}
		}
		else{
			wait(NULL);
			child_num++;
		}
	}
	if(child){
		int sock;
		if(argv[1])
			sock=init_server("1.1.1.1",atoi(argv[1]));
		else
			sock=init_server("1.1.1.1",8888);
		server_loopwork(sock);
	}
}
