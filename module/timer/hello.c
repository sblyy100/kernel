#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/timer.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SBL");
MODULE_DESCRIPTION("timer test");
struct timer_list t;
static void func(unsigned long data){
	printk(KERN_ERR "timer print\n");
	mod_timer(&t,jiffies+10*HZ);
}
static int __init hello_init(void)
{
	printk(KERN_ERR "hello world!\n");
	init_timer(&t);
	t.expires = jiffies + 10*HZ;
	t.function = func;
	t.data = (unsigned long)NULL;
	add_timer(&t);
	/*
	g_task = kthread_create(print_task,MULL,"sbl_thread");
	if(!IS_ERR(g_task)){
		wake_up_process(g_task);
	}
	*/
	return 0;
}
static void __exit hello_exit(void)
{
	//if(g_task)
	//	kthread_stop(g_task);
	del_timer_sync(&t);
    printk(KERN_EMERG "hello exit!\n");
}
module_init(hello_init);
module_exit(hello_exit);
