#ifndef _LIST
#define _LIST
#include <stdlib.h>
struct list_head{
	struct list *prev,*next;
};
struct list{
	struct list_head head;
	void *data;
};
void init_list(struct list_head *head){
	head->prev = NULL;
	head->next = NULL;
}
void list_add(struct list_head *head,struct list *elem){
	struct list_head *pos = &elem->head;
	pos->next = head->next;
	pos->prev = head;
	head->next = pos;
}
void list_add_tail(struct list_head *head,struct list *elem){
	struct list_head *pos = &elem->head;
	struct list_head *tail = head->prev;
	pos->next = head;
	pos->prev = tail;
	tail->next = pos;
	head->prev = pos;
}
#endif
