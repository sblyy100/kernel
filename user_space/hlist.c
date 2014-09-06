#include <stdlib.h>
#include <stdio.h>
#include <linux/list.h>
#define LISTSIZE 1024
struct test{
	int data;
	struct hlist_node node;
};
unsigned int gethash(int c)
{
    return (c & 0xff);
} 

int main(){
	int hash=0;
	int i;
	struct test t[10]={{3,},{4,},{5,},{6,},{7,},{8,},{9,},{10,},{11,},{12,}};
	struct test *tmp;
	struct hlist_node *pos;
	printf("%s\n","helloworld");
	struct hlist_head hlist[1024];
	for(hash = 0; hash < LISTSIZE; hash++)
		INIT_HLIST_HEAD(&hlist[hash]); 
	
	for(i=0;i<10;i++){
		hash = gethash(t[i].data);
		hlist_add_head(&t[i].node, &hlist[hash]); 
	}
	for(i=0;i<LISTSIZE;i++){
		hlist_for_each(pos,&hlist[i]){
			tmp = hlist_entry(pos,struct test,node);//ptr,type,member
			printf("data:%d\n",tmp->data);
		}
	}


} 	
