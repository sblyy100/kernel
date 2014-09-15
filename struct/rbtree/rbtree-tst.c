/*
 * =============================================================================
 *
 *       Filename:  rbtree-tst.c
 *
 *    Description:  rbtree testcase.
 *
 *        Created:  09/02/2012 11:39:34 PM
 *
 *         Author:  Fu Haiping (forhappy), haipingf@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */

#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct mynode {
  	struct rb_node node;
  	char *key;
	char *value;
};

struct rb_root mytree = RB_ROOT;

struct mynode * my_search(struct rb_root *root, char *key)
{
  	struct rb_node *node = root->rb_node;

  	while (node) {
  		struct mynode *data = rb_entry(node, struct mynode, node);
		int result;

		result = strcmp(key, data->key);

		if (result < 0)
  			node = node->rb_left;
		else if (result > 0)
  			node = node->rb_right;
		else
  			return data;
	}
	return NULL;
}

int my_insert(struct rb_root *root, struct mynode *data)
{
  	struct rb_node **new = &(root->rb_node), *parent = NULL;

  	/* Figure out where to put new node */
  	while (*new) {
  		struct mynode *this = container_of(*new, struct mynode, node);//equal to rb_entry
  		int result = strcmp(data->key, this->key);

		parent = *new;
  		if (result < 0)
  			new = &((*new)->rb_left);
  		else if (result > 0)
  			new = &((*new)->rb_right);
  		else
  			return 0;
  	}

  	/* Add new node and rebalance tree. */
  	rb_link_node(&data->node, parent, new);
  	rb_insert_color(&data->node, root);

	return 1;
}

void my_free(struct mynode *node)
{
	if (node != NULL) {
		if (node->key != NULL) {
			free(node->key);
			node->key = NULL;
		}
		if (node->value != NULL) {
			free(node->value);
			node->value = NULL;
		}
		free(node);
		node = NULL;
	}
}

#define NUM_NODES 32

int main()
{

	struct mynode *mn[NUM_NODES];
	
	/* *insert */
	int i = 0;
	printf("insert node from 1 to NUM_NODES(32): \n");
	for (; i < NUM_NODES; i++) {
		mn[i] = (struct mynode *)malloc(sizeof(struct mynode));
		mn[i]->key = (char *)malloc(sizeof(char) * 4);
		mn[i]->value = (char *)malloc(sizeof(char) * 4);
		sprintf(mn[i]->key, "%d", i);
		sprintf(mn[i]->value, "sb%d", i);
		my_insert(&mytree, mn[i]);
	}
	
	
	/* *search */
	struct rb_node *node;
	struct mynode *my;
	printf("search all nodes: \n");
	for (node = rb_first(&mytree); node; node = rb_next(node)){
		my = rb_entry(node,struct mynode,node);
		printf("key = %s,value = %s\n", my->key,my->value);
	}
	/* *delete */
	printf("delete node 20: \n");
	struct mynode *data = my_search(&mytree, "20");
	if (data) {
		rb_erase(&data->node, &mytree);
		my_free(data);
	}

	/* *delete again*/
	printf("delete node 10: \n");
	data = my_search(&mytree, "10");
	if (data) {
		rb_erase(&data->node, &mytree);
		my_free(data);
	}

	/* *delete once again*/
	printf("delete node 15: \n");
	data = my_search(&mytree, "15");
	if (data) {
		rb_erase(&data->node, &mytree);
		my_free(data);
	}

	/* *search again*/
	printf("search again:\n");
	for (node = rb_first(&mytree); node; node = rb_next(node)){
		my = rb_entry(node,struct mynode,node);
		printf("key = %s,value = %s\n", my->key,my->value);
	}
	/*replace 14 with data sss4*/
	struct mynode *new_node;
		new_node = (struct mynode *)malloc(sizeof(struct mynode));
		new_node->key = (char *)malloc(sizeof(char) * 4);
		new_node->value = (char *)malloc(sizeof(char) * 4);
		sprintf(new_node->key,"%d",14);
		sprintf(new_node->value,"%s","sss4");
	data = my_search(&mytree, "14");
	if(data){
		rb_replace_node(&data->node,new_node,&mytree);
		my_free(data);
	}
	/* *search again*/
	printf("search again:\n");
	for (node = rb_first(&mytree); node; node = rb_next(node)){
		my = rb_entry(node,struct mynode,node);
		printf("key = %s,value = %s\n", my->key,my->value);
	}
	return 0;
}


