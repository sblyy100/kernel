#include "online.h"
/*no head node*/
online_list add_online(online_list first,online_list cur,int sock){
	online_list new_node;
	new_node=malloc(sizeof(online_node));
	new_node->client_sock=sock;
	new_node->next=NULL;
	cur->next=new_node;
	cur=new_node;
	(first->size)++;
	return cur;
}
online_list del_online(online_list first,int sock){
	online_node *off_node,*tmp,*cur;
	/*init :cur is the pre of off_node*/
	off_node=first->next;
	cur=first;
	while(off_node!=NULL){
	if(off_node->client_sock!=sock){
		cur=off_node;
		off_node=off_node->next;
		continue;
		}//end if
	else{
		tmp=off_node;
		cur->next=tmp->next;
		free(tmp);
		(first->size)--;
		break;
	}
	}//end while
	
	return cur; 
}
online_list check_online(online_list first){
	online_list check=first;	
	while(check){
		printf("sock:%d  ",check->client_sock);
		check=check->next;
	}
	printf("\n");
return first;
}
