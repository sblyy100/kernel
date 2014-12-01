#ifndef _SERVER_H
#define _SERVER_H
#include "myhead.h"
#include <poll.h>
#include <sys/epoll.h>
int init_server(char *ip,int port);
//int init_server(struct sockaddr);
void server_loopwork(int sock);
void worker_thread();
void thread_exit_handler(int);
void listen_thread(int);
#endif
