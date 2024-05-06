//Support for int&str&array parameter
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>

#define MAX_ARRAY 6
static int int_var = 0;
static char *str_var = "default";
static int int_array[6];
int narr;
module_param(int_var, int, 0644);
MODULE_PARM_DESC(int_var, "A integer variable");

module_param(str_var, charp, 0644);
MODULE_PARM_DESC(str_var, "A string variable");
module_param_array(int_array, int, &narr, 0644);
MODULE_PARM_DESC(int_array, "A integer array");
 
static int __init hello_init(void)
{
	int i;
	printk(KERN_INFO "Hello Module2\n");
	printk(KERN_INFO "int_var is %d.\n", int_var);
	printk(KERN_INFO "str_var is %s.\n", str_var);
	for(i = 0; i < narr; i ++){
		printk(KERN_INFO "int_array[%d] = %d\n", i, int_array[i]);
	}
	return 0;
}
static void __exit hello_exit(void)
{
       printk(KERN_INFO "GoodBye Module2\n");
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
