#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#define PROC_FILENAME "hello_proc"

//定义读取函数
static int my_proc_show(struct seq_file *m, void *v) {
	//将字符串传入`seq_file`结构中，`seq_file`会在 `/proc` 文件的读取操作中使用
	seq_printf(m, "Hello from my_proc_file!\n");
	return 0;
}
//该函数被声明为 `static`，表示它只在当前文件中可见。
//- `inode`：指向 inode 结构的指针，表示要打开的文件的元数据信息。
// `file`：指向 file 结构的指针，表示正在打开的文件的实例。
static int my_proc_open(struct inode *inode, struct file *file) {
	//- `file` 参数是要打开的文件的文件实例。
	//- `my_proc_show` 是一个回调函数，用于在读取文件时提供数据。
	//- `NULL` 是一个可选的私有数据指针，这里没有使用，所以设置为 NULL。
	return single_open(file, my_proc_show, NULL);
}
//为了将读取函数my_proc_show和其他文件操作函数连接起来，需要定义一个 `file_operations` 结构体，
//并将你读取函数赋值给 `read` 字段。
static const struct file_operations my_proc_fops = {
	.owner = THIS_MODULE,//指向拥有这个文件操作结构的模块的指针
	.open = my_proc_open,//文件打开时调用的函数
	.read = seq_read,//读取, 当文件被read的时候，调用seq_read,其再调用.show接口
	                 //（在这个模块中.show接口就是my_proc_show，在my_proc_open提到了）
	/*
	`seq_read` 函数是 Linux 内核中用于顺序读取的通用函数，
	它主要用于处理 proc 文件系统中的文件读取请求。
	当用户空间程序调用 `read` 系统调用来读取 proc 文件时，
	内核将会调用 `seq_read` 函数来处理这个请求
	*/
	.llseek = seq_lseek,//定位
	.release = single_release,//释放
};

static int __init my_module_init(void) {
	//建立`\proc`文件
	//(文件名，权限，父目录，文件操作结构体)
	//user、group 和 others, 4读2写1执行，0无
	proc_create(PROC_FILENAME, 0444, NULL, &my_proc_fops);
	printk(KERN_INFO "Hello Module3\n");
	return 0;
	
}

static void __exit my_module_exit(void) {
	//移除`\proc`文件
	remove_proc_entry(PROC_FILENAME, NULL);
	printk(KERN_INFO "GoodBye Module3\n");
}

module_init(my_module_init);
module_exit(my_module_exit);
MODULE_LICENSE("GPL");

