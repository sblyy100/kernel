#include <linux/module.h>
#include <linux/gfp.h>
#include <linux/mm.h>
#include <linux/slab.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SBL");
MODULE_DESCRIPTION("kmem_cache");
struct kmem_cache *kc;
struct obj{
	int k;
	int v;
};
static int __init hello_init(void)
{
	struct obj *ol[20] = {NULL,};
	int i;
	kc = kmem_cache_create("test",10*sizeof(struct obj),0,0,NULL);
	if(!kc){
		printk(KERN_ERR"cache alloc failed\n");
		return -1;
	}
	for(i=0;i<15;i++){
		ol[i] = kmem_cache_alloc(kc,GFP_KERNEL);
		if(ol[i]){
			ol[i]->k = i;
			ol[i]->v = i*10;
		}
	}
	for(i=0;i<15;i++){
		if(ol[i])
			printk(KERN_ERR"k=%d,v=%d\n",ol[i]->k,ol[i]->v);
	}
	for(i=0;i<15;i++){
		if(ol[i]){
			kmem_cache_free(kc,ol[i]);
			printk(KERN_ERR"free node:%d\n",i);
		}
	}
	return 0;
}
static void __exit hello_exit(void)
{
	if(kc)
		kmem_cache_destroy(kc);
    printk(KERN_EMERG "hello exit!\n");
}
module_init(hello_init);
module_exit(hello_exit);
