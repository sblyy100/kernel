#include <linux/module.h>
#include <linux/init.h>
#include <linux/idr.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SBL");
MODULE_DESCRIPTION("struct");
struct idr my_idr;
char *p = "hello";
static int __init hello_init(void)
{
	int ret;
	int id;
	char *p1;
	idr_init(&my_idr);
	if(!idr_pre_get(&my_idr,GFP_KERNEL))
		return -ENOSPC;
	ret = idr_get_new(&my_idr,p,&id);
	printk("new UID:%d\n",id);
	p1 = idr_find(&my_idr,id);
	if(!p1)
		return -EINVAL;
	printk(KERN_INFO "p:0x%x,p1:0x%x\n",p,p1);
	idr_remove(&my_idr,id);

	return 0;
}
static void __exit hello_exit(void)
{
    printk(KERN_INFO "hello exit!\n");
	idr_remove_all(&my_idr);
	idr_destory(&my_idr);
}
module_init(hello_init);
module_exit(hello_exit);
