//Test for installing and removing of module.
#include <linux/init.h> 
#include <linux/module.h> 
#include <linux/kernel.h>
static int __init hello_init(void){
	printk(KERN_INFO "Hello Module1\n"); 
	return 0;
}

static void __exit hello_exit(void){ 
	printk(KERN_INFO "GoodBye Module1\n");
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
