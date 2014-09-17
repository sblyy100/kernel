#include "server.h"
#include "myhead.h"
#include "online.h"
#include "log.h"
#include "response.h"
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
	char buf_r[BUFF]="\0";//stack is 8M
	char *response="HTTP/1.1 200 OK \nContent-length:3\n\naaa\0";
	struct sockaddr_in client;
	int newsock;
	int client_len=sizeof(client);
	
	struct epoll_event ev,events[MAXFDS];
	int epfd;//epoll_create(n) will return a fd 
	/*
		struct epoll_event {
			__uint32_t events; 
			epoll_data_t data; 
};
	*/
	int i;//循环找出空闲的fd
	int tmpsock;
	online_node *worknode,*tmp;
	int timeout = 1000;//ms
	if(listen(sock,MAXCON)<0){
        mylog("listen error\n",2,__FILE__,__LINE__);
        return ;
        }//endif
	ev.data.fd=sock;
	ev.events=EPOLLIN | EPOLLET;
	epfd=epoll_create(MAXFDS);
	epoll_ctl(epfd, EPOLL_CTL_ADD, sock,&ev);
	while(1)
	{	
		/*将epfd映射到了epfds*/
		if(epoll_wait(epfd, events, MAXFDS, 1)<=0){
			//mylog("select error\n",2,__FILE__,__LINE__);
			continue;
		}
		for(i=0;i<MAXFDS;i++){
			if((events[i].data.fd==sock)){
				if(events[i].events&EPOLLIN){
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
				
					ev.data.fd=newsock;
					ev.events=EPOLLIN | EPOLLET;
					epoll_ctl(epfd, EPOLL_CTL_ADD, newsock,&ev);
					#ifdef DEBUG
					printf("newsock %d,first size is %d\n",newsock,first->size);
					#endif
					check_online(first); //print all the sock 
				}
				continue;
			}
			
			
			// #ifdef DEBUG
			// printf("start work,first size is %d\n",first->size);
			// #endif
			/*here can use FD_CLR*/		
		
			if((tmpsock=events[i].data.fd)<0){
				printf("=======================UNSET\n");
				continue;
			}
			if(events[i].events&EPOLLHUP){
				printf("=======================RESET\n");
				cur=del_online(first,tmpsock);
				epoll_ctl(epfd, EPOLL_CTL_DEL, tmpsock,&ev);
				#ifdef DEBUG
				printf("delete sock:%d,now size is %d,thread id: %u\n",tmpsock,first->size,pthread_self());
				#endif
				close(tmpsock);
				continue;
			}
			if(events[i].events&EPOLLIN){
				if(recv(tmpsock,buf,BUFF,0)>0){
					printf("=======================READ\n");
					#ifdef DEBUG
					printf("recv buf:%s from thread id: %u\n",buf,pthread_self());
					#endif
					mylog(buf,2,__FILE__,__LINE__);
					return_response(buf,buf_r,BUFF);
					send(tmpsock,buf_r,strlen(buf_r),0);	
					continue;
				}
				else{
					printf("=======================CLOSE\n");
					cur=del_online(first,tmpsock);
					epoll_ctl(epfd, EPOLL_CTL_DEL, tmpsock,&ev);
					#ifdef DEBUG
					printf("delete sock:%d,now size is %d,thread id: %u\n",tmpsock,first->size,pthread_self());
					#endif
					close(tmpsock);
					/*
						如果不关闭，服务器进入close_wait状态，客户端进入fin_wait2状态
					*/
					continue;
				}
			}	
		
			// #ifdef DEBUG
			// printf("next accept loop\n");
			// check_online(first);
			// #endif
		
		}//end for
	}//end while
	close(epfd);
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
