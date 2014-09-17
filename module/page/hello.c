#include <linux/module.h>
#include <linux/gfp.h>
#include <linux/mm.h>
#include <linux/highmem.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SBL");
MODULE_DESCRIPTION("page");
static int __init hello_init(void)
{
	struct page *page,*page_high;
	char *p,*p2 = NULL;
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
	p = NULL;

	page_high = alloc_pages(GFP_HIGHUSER,0);//2^0=1
	if(!page_high){
		printk(KERN_ERR"high mem alloc failed\n");
		return -1;
	}
	p = kmap(page_high);
	if(!p){
		printk(KERN_ERR"high page kmap failed\n");
		return -1;
	}
	p2 = page_address(page_high);
	if(!p2){
		printk(KERN_ERR"can't use page_address for high page\n");
	}else{
		printk(KERN_ERR "high page:%p,virtual addr:%p!\n",page_high,p2);
	}
	printk(KERN_ERR "high page:%p,virtual addr:%p!\n",page_high,p);
	memcpy(p,"hello",sizeof("hello"));
	printk(KERN_ERR"page string:%s\n",p);
	kunmap(page_high);
	__free_pages(page_high,0);
	return 0;
}
static void __exit hello_exit(void)
{
    printk(KERN_EMERG "hello exit!\n");
}
module_init(hello_init);
module_exit(hello_exit);
