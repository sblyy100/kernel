#include <linux/module.h>
#include <asm/atomic.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/preempt.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SBL");
MODULE_DESCRIPTION("preempt");
static int __init hello_init(void)
{
	int cpu;
	cpu = get_cpu();//preempt_disable();
	preempt_disable();
	printk(KERN_ERR"cpu:%d,preempt count:%u\n",cpu,preempt_count());

	printk(KERN_ERR "hello world!\n");
	return 0;
}
static void __exit hello_exit(void)
{
    printk(KERN_EMERG "hello exit!\n");
	preempt_enable();
	put_cpu();//preempt_enable();
}
module_init(hello_init);
module_exit(hello_exit);
