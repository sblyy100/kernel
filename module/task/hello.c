#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/kthread.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SBL");
MODULE_DESCRIPTION("Task info");
int print_task(void);
int print_task(){
	struct task_struct *task;

	for_each_process(task){
		printk(KERN_INFO "name:%s pid:%d tgid:%d prio:%d state:%d\n",task->comm,task->pid,task->tgid,task->prio,task->state);
	}
	return 0;
}
static int __init hello_init(void)
{
	printk(KERN_ERR "hello world!\n");
	kthread_run(print_task,NULL,"sbl_thread");
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
    printk(KERN_EMERG "hello exit!\n");
}
module_init(hello_init);
module_exit(hello_exit);
