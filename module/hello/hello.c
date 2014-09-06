#include <linux/module.h>
#include <linux/init.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("DAVID");
MODULE_DESCRIPTION("Hello world module");
static int __init hello_init(void)
{
    printk(KERN_ERR "hello world!\n");
    return 0;
}
static void __exit hello_exit(void)
{
    printk(KERN_EMERG "hello exit!\n");
}
module_init(hello_init);
module_exit(hello_exit);
