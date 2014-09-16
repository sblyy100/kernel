#include <linux/module.h>
#include <linux/gfp.h>
#include <linux/mm.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SBL");
MODULE_DESCRIPTION("page");
static int __init hello_init(void)
{
	struct page *page;
	char *p;
	printk(KERN_ERR "hello world!\n");
	//p = __get_free_pages(GFP_KERNEL,3);
	//page = alloc_pages(GFP_KERNEL,3);
	p = get_zeroed_page(GFP_KERNEL);
	if(!p){
		printk(KERN_ERR "no free pages!\n");
		return -1;
	}
	//p = page_address(page);
	page = virt_to_page(p);
	printk(KERN_ERR "page:%p,page addr:%p!\n",page,p);
	memcpy(p,"hello",sizeof("hello"));
	printk(KERN_ERR"page string:%s\n",p);
	//free_pages(p,3);
	//free_page(p);
	__free_pages(page,0);
	return 0;
}
static void __exit hello_exit(void)
{
    printk(KERN_EMERG "hello exit!\n");
}
module_init(hello_init);
module_exit(hello_exit);
