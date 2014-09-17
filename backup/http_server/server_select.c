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


//head->next=NULL; //init here is error

//static onlinelist online[100];//the best is lianbiao
//onlinelist online[100];//the best is lianbiao

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
void worker_thread(){
	char buf[BUFF]="\0";
	char *response="test\n\0";
	int tmpsock;
	online_node *worknode,*tmp;
#ifdef DEBUG
	 printf("this in work thread\n");
	 mylog("worker thread start\n",2,__FILE__,__LINE__);
	 //pthread_cleanup_push(thread_exit_handler,newsock);
#endif
	/*int select(nfds, &readfds, &writefds, &exceptfds, timeout) */
	
		/* if((first->size)<=0){
			#ifdef DEBUG
			printf("no work,return to accept,first size is %d\n",first->size);
			#endif
			continue;
		} */
		#ifdef DEBUG
		printf("start work,first size is %d\n",first->size);
		#endif
		
		/*here can use FD_CLR*/		
		switch(select(MAXFDS,&rfds,NULL,NULL,NULL)){
			case -1:
				#ifdef DEBUG
				printf("select error, thread id: %u\n",pthread_self());
				#endif
				break;
			case 0:
				#ifdef DEBUG
				printf("select return 0, thread id: %u\n",pthread_self());
				//sleep(1);
				#endif
				break;
			default:
				#ifdef DEBUG
				printf("select ok, thread id: %u\n",pthread_self());
				#endif
				worknode=first->next;
				while(worknode&&first->size>0){
					tmpsock=worknode->client_sock;
					if(FD_ISSET(tmpsock,&rfds)){
						if(recv(tmpsock,buf,BUFF,0)>0){	
							#ifdef DEBUG
							printf("recv buf:%s from thread id: %u\n",buf,pthread_self());
							#endif
							send(tmpsock,response,strlen(response),0);	
						}else{
							FD_CLR(tmpsock,&rfds);
							cur=del_online(first,tmpsock);
							#ifdef DEBUG
							printf("delete sock:%d,now size is %d,thread id: %u\n",tmpsock,first->size,pthread_self());
							#endif
							close(tmpsock);
						}
					}
					worknode=worknode->next;
				}
				break;
		}//end switch	
		return;
}
void listen_thread(int sock){
	fd_set rfds,allfds; 
	char buf[BUFF]="\0";
	char *response="HTTP/1.1 200 OK \n\naaa\0";
	int sel;
	struct sockaddr_in client;
	int newsock;
	int client_len=sizeof(client);
	
	int tmpsock;
	online_node *worknode,*tmp;
	struct timeval timeout={1,1};
	if(listen(sock,MAXCON)<0){
        mylog("listen error\n",2,__FILE__,__LINE__);
        return -1;
        }//endif
	FD_ZERO(&allfds); 
	FD_SET(sock,&allfds);
	//first->size<MAXFDS
	while(1)
	{	
		timeout.tv_sec=1;
		
		/*here can select too*/
		rfds=allfds;//保证每次都是未改变状态的
		sel=select(MAXFDS,&rfds,NULL,NULL,&timeout);
		if(sel<=0){
			mylog("select error\n",2,__FILE__,__LINE__);
			continue;
		}

			if(FD_ISSET(sock,&rfds)){
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
				FD_SET(newsock,&allfds);
				
				#ifdef DEBUG
				printf("newsock %d,first size is %d\n",newsock,first->size);
				#endif
				check_online(first); //print all the sock 
			}
			

			#ifdef DEBUG
			printf("start work,first size is %d\n",first->size);
			#endif
			worknode=first->next;
			/*here can use FD_CLR*/		
		while(1){
			if(worknode!=NULL){
				tmpsock=worknode->client_sock;
				if(FD_ISSET(tmpsock,&rfds)){
					if(recv(tmpsock,buf,BUFF,0)>0){	
						#ifdef DEBUG
						printf("recv buf:%s from thread id: %u\n",buf,pthread_self());
						#endif
						send(tmpsock,response,strlen(response),0);	
						// FD_CLR(tmpsock,&allfds);
						// cur=del_online(first,tmpsock);
						// close(tmpsock);
					}
					else{
						FD_CLR(tmpsock,&allfds);
						cur=del_online(first,tmpsock);
						#ifdef DEBUG
						printf("delete sock:%d,now size is %d,thread id: %u\n",tmpsock,first->size,pthread_self());
						#endif
						close(tmpsock);
						}
				}
						worknode=worknode->next;
			}
			else
				break;
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