#include "list.h"
#include <stdlib.h>
#include <stdio.h>
int main(){
	struct list_head head,*pos=&head;
	int i;
	init_list(&head);
	struct list *elem;
	for(i=0;i<10;i++){
		elem = malloc(sizeof(struct list));
		if(!elem)
			perror("malloc failed");
		elem->data = malloc(10*sizeof(char));
		if(!elem->data)
			perror("malloc data failed");
		sprintf(elem->data,"hello%d",i);
		list_add(&head,elem);
	}
	for(i=0;i<10;i++){
		elem = pos->next;
		if(elem)
			printf("data:%s\n",(char*)elem->data);
		pos = pos->next;
	}
}
