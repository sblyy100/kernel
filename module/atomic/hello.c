#include <linux/module.h>
#include <asm/atomic.h>
#include <linux/slab.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SBL");
MODULE_DESCRIPTION("atomic");
struct test{
	atomic_t u;
	char *data;
}gtest;
static void init_test(struct test* gtest){
	atomic_set(&gtest->u,1);
	gtest->data = kmalloc(10*sizeof(char),GFP_KERNEL);
	memcpy(gtest->data,"hello",sizeof("hello"));
}
static void use_test(struct test* gtest){
	atomic_inc(&gtest->u);
}
static int free_test(struct test* gtest){
	if(atomic_dec_and_test(&gtest->u)){
		kfree(gtest->data);
		printk(KERN_ERR"free data:%d\n",atomic_read(&gtest->u));
		return 1;
	}
	printk(KERN_ERR"not free:%d\n",atomic_read(&gtest->u));
	return 0;
}
static int __init hello_init(void)
{
	/*
	atomic_t v = ATOMIC_INIT(0);
	printk(KERN_ERR"v:%d\n",atomic_read(&v));
	atomic_set(&v,4);
	printk(KERN_ERR"v:%d\n",atomic_read(&v));
	atomic_add(1,&v);
	printk(KERN_ERR"v:%d\n",atomic_read(&v));
	atomic_inc(&v);
	printk(KERN_ERR"v:%d\n",atomic_read(&v));
	atomic_dec(&v);
	printk(KERN_ERR"v:%d\n",atomic_read(&v));
	printk(KERN_ERR"v:%d\n",atomic_add_return(2,&v));
	*/
	//init
	init_test(&gtest);
	use_test(&gtest);
	use_test(&gtest);
	use_test(&gtest);

	printk(KERN_ERR "hello world!\n");
	return 0;
}
static void __exit hello_exit(void)
{
	int ret = 0;
    printk(KERN_EMERG "hello exit!\n");
	do{
		ret = free_test(&gtest);
	}while(ret != 1);
}
module_init(hello_init);
module_exit(hello_exit);
