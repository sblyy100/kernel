#include <linux/module.h>
#include <asm/atomic.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/list.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SBL");
MODULE_DESCRIPTION("spinlock");
spinlock_t lock;
struct list_head head;
struct test{
	struct list_head list;
	int data;
};
static struct test* init_node(int i){
	struct test *t;
	t = kmalloc(sizeof(struct test),GFP_KERNEL);
	if(t){
		INIT_LIST_HEAD(&t->list);
		t->data = i;
		return t;
	}
	return NULL;
}
static void init_test(struct list_head *head,spinlock_t *lock){
	int i;
	struct test *tmp;
	spin_lock_init(lock);
	INIT_LIST_HEAD(head);
	for(i=0;i<10;i++){
		tmp = init_node(i);
		if(tmp){
			list_add(&tmp->list,head);
		}
	}
}
static void print_node(struct list_head *head){
	struct list_head *pos;
	struct test *t;
	list_for_each(pos,head){
		t = list_entry(pos,struct test,list);
		printk(KERN_ERR"%d\n",t->data);
	}
}
static int free_test(struct list_head *head){
	struct list_head *pos,*n;
	struct test *t;
	spin_lock(&lock);
	list_for_each_safe(pos,n,head){
		t = list_entry(pos,struct test,list);
		list_del(&t->list);
		if(t)
			kfree(t);
	}
	spin_unlock(&lock);

	return 0;
}
static int __init hello_init(void)
{
	//init
	
	init_test(&head,&lock);
	print_node(&head);
	//add
	struct test *t;
	t = init_node(11);
	spin_lock(&lock);
	list_add(&t->list,&head);
	spin_unlock(&lock);
	print_node(&head);

	printk(KERN_ERR "hello world!\n");
	return 0;
}
static void __exit hello_exit(void)
{
    printk(KERN_EMERG "hello exit!\n");
	free_test(&head);
}
module_init(hello_init);
module_exit(hello_exit);
