#ifndef _UTIL_H
#define _UTIL_H
#include "myhead.h"

typedef	struct{
    int client_sock;
	//char name[32];
        //char passwd[32];
        //char number[12];
        //char desc[100];
	volatile int size;
	struct online_node *next; 
	//only the first size is true
}online_node,*online_list;
//typedef online_node *online_list;
online_list add_online(online_list,online_list,int);
online_list del_online(online_list,int);
online_list check_online(online_list);
online_list get_online_element(online_first);

#endif
