#include <linux/module.h>
#include <linux/init.h>
#include <linux/kfifo.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SBL");
MODULE_DESCRIPTION("struct");
struct kfifo fifo;
static int __init hello_init(void)
{
	int ret;
	unsigned int l,s,a;
	char buf[100]={0};
	ret = kfifo_alloc(&fifo,PAGE_SIZE,GFP_KERNEL);
	if(ret)
		return ret;
	s = kfifo_size(&fifo);
	l = kfifo_len(&fifo);
	a = kfifo_avail(&fifo);
	printk(KERN_INFO "s,l,a=%u %u %u\n",s,l,a);
	kfifo_in(&fifo,"hello",5);
	kfifo_out_peek(&fifo,buf,4);
	printk(KERN_INFO "buf:%s\n",buf);
	s = kfifo_size(&fifo);
	l = kfifo_len(&fifo);
	a = kfifo_avail(&fifo);
	printk(KERN_INFO "s,l,a=%u %u %u\n",s,l,a);
	kfifo_out(&fifo,buf,3);
	printk(KERN_INFO "buf:%s\n",buf);
	s = kfifo_size(&fifo);
	l = kfifo_len(&fifo);
	a = kfifo_avail(&fifo);
	printk(KERN_INFO "s,l,a=%u %u %u\n",s,l,a);
	return 0;
}
static void __exit hello_exit(void)
{
    printk(KERN_INFO "hello exit!\n");
		kfifo_reset(&fifo);
		kfifo_free(&fifo);
}
module_init(hello_init);
module_exit(hello_exit);
