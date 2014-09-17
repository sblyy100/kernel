#include "server.h"
#include "myhead.h"
#include "online.h"
#include "log.h"
#define MAXCON 100
#define MAXFDS 128
#define BUFF 4096
extern errno;
static void sig_handler(int);//避免defunct 
//online_list head;
online_list first,cur;

int init_server(char *ip,int port){
	int sock;
	struct sockaddr_in server;
	server.sin_family=AF_INET;    ///init server addr
    server.sin_addr.s_addr=htonl(INADDR_ANY);
    server.sin_port=htons(port);
    memset(server.sin_zero,0,8);
	if((sock=socket(AF_INET,SOCK_STREAM,0))<0){
		mylog("create sock error\n",2,__FILE__,__LINE__);
		return -1;
	}
	mylog("create sock sucessful\n",2,__FILE__,__LINE__);
	
	if(bind(sock,(struct sockaddr*)&server,sizeof(server))<0){
		mylog("bind sock error\n",2,__FILE__,__LINE__);
		return -1;
	}
	mylog("bind sock sucessful\n",2,__FILE__,__LINE__);
	
	first=malloc(sizeof(online_node));
	cur=first;
	first->client_sock=sock;
	first->next=NULL;
	first->size=0;
	
	return sock;
}
void server_loopwork(int sock){
	pthread_t tid;	
	
	 //listen thread
	// if(pthread_create(&tid,NULL,listen_thread,sock)<0){
		// mylog("create listen thread error",2,__FILE__,__LINE__);
	// }
	listen_thread(sock);
	//mylog("create listen thread ok",2,__FILE__,__LINE__);
	
	
	// pthread_join(tid,NULL);
	printf("I can't at here\n");
	close(sock);//close server socket
}
void listen_thread(int sock){
	char buf[BUFF]="\0";
	char *response="HTTP/1.1 200 OK \nContent-length:3\nConnection:close\n\naaa\0";
	struct sockaddr_in client;
	int newsock;
	int client_len=sizeof(client);
	
	struct pollfd pfds[MAXFDS];
	/*
		struct pollfd{
			int fd;
			short events;///POLLIN(read),POLLOUT(write),POLLHUP(close)
			short revents;
		}
	*/
	int i;//循环找出空闲的fd
	int tmpsock;
	online_node *worknode,*tmp;
	int timeout = 1000;//ms
	if(listen(sock,MAXCON)<0){
        mylog("listen error\n",2,__FILE__,__LINE__);
        return -1;
        }//endif
	pfds[0].fd=sock;
	pfds[0].events=POLLIN;
	while(1)
	{	
		
		if(poll(pfds,MAXFDS,timeout)<=0){
			mylog("select error\n",2,__FILE__,__LINE__);
			continue;
		}
		if(pfds[0].revents&POLLIN){
				if((newsock=accept(sock,(struct sockaddr*)&client,&client_len))<0){
					mylog("accept error in listen thread",2,__FILE__,__LINE__);
					exit(0);
				}//end if
				mylog("accept success in listen thread",2,__FILE__,__LINE__);
				sprintf(buf,"accept a connection fron %s:%d\n",inet_ntoa(client.sin_addr.s_addr),ntohs(client.sin_port));
				mylog(buf,2,__FILE__,__LINE__);
				/*work may delete some sock,so*/
				#ifdef DEBUG
				printf("newsock %d,first size is %d\n",newsock,first->size);
				#endif
				
				cur=add_online(first,cur,newsock);
				for(i=1;i<MAXFDS;i++){
					if(pfds[i].fd>0)//how to init
						continue;
					pfds[i].fd=newsock;
					pfds[i].events=POLLIN;
					break;
				}
				
				#ifdef DEBUG
				printf("newsock %d,first size is %d\n",newsock,first->size);
				#endif
				check_online(first); //print all the sock 
			}
			

			#ifdef DEBUG
			printf("start work,first size is %d\n",first->size);
			#endif
			/*here can use FD_CLR*/		
		for(i=1;i<MAXFDS;i++){
			if((tmpsock=pfds[i].fd)<0)
				continue;
			if(pfds[i].revents&POLLHUP){
				
				cur=del_online(first,tmpsock);
				pfds[i].fd=-1;
				#ifdef DEBUG
				printf("delete sock:%d,now size is %d,thread id: %u\n",tmpsock,first->size,pthread_self());
				#endif
				close(tmpsock);
			}
			if(pfds[i].revents&POLLIN){
				if(recv(tmpsock,buf,BUFF,0)>0)
					#ifdef DEBUG
					printf("recv buf:%s from thread id: %u\n",buf,pthread_self());
					#endif
					send(tmpsock,response,strlen(response),0);	
			}			
		}
			#ifdef DEBUG
			printf("next accept loop\n");
			check_online(first);
			#endif
			//end switch	
	}//end while
}

void thread_exit_handler(int sock){
	printf("thread %u exit\n",pthread_self());
	close(sock);
}

void sig_handler(int sig){
	online_list tmp;
	switch(sig){
		case SIGCLD:
			printf("clild exit...\n");
			wait(NULL);
		case SIGINT:
			break;
		default:
			break;
	}
}