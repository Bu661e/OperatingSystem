#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>  // 包含copy_from_user函数所需的头文件

#define HELLO_DIR "hello_dir"
#define HELLO_FILE "hello"
#define MAX_VAL_LEN 20  // 设置最大的值长度，以防止缓冲溢出

//内核空间缓冲区
static char hello_val[MAX_VAL_LEN + 1];  // 可读可写的文件的值

static int hello_show(struct seq_file *m, void *v) {
	seq_printf(m, "hello_val = %s\n", hello_val);
	return 0;
}

static int hello_open(struct inode *inode, struct file *file) {
	return single_open(file, hello_show, NULL);
}

//目标是将用户提供的数据从用户空间缓冲区写入到内核空间缓冲区(hello_vel)中。
//文件指针 `file`，指向用户提供的数据缓冲区的指针 `buffer`，要写入的字节数 `count`
//，以及文件偏移量 `f_pos`它用于指示写入操作应从文件的哪个位置开始写入数据。
static ssize_t hello_write(struct file *file, const char __user *buffer, size_t count, loff_t *f_pos) {
	
	if (count > MAX_VAL_LEN)
		return -EINVAL;  // 返回错误，表示写入的数据太长
	
	if (copy_from_user(hello_val, buffer, count))  // 从用户空间复制数据到内核空间
		return -EFAULT;  // 返回错误，表示从用户空间复制数据时出错
	
	hello_val[count] = '\0';  // 添加字符串结束符
	
	return count;
}

static const struct file_operations hello_fops = {
	.owner = THIS_MODULE,
	.open = hello_open,
	.read = seq_read,
	.write = hello_write,
	.llseek = seq_lseek,
	.release = single_release,
};

static int __init my_module_init(void) {
	struct proc_dir_entry *hello_dir;
	
	// 创建目录 /proc/hello_dir
	hello_dir = proc_mkdir(HELLO_DIR, NULL);
	if (!hello_dir) {
		printk(KERN_ERR "Failed to create /proc/%s directory\n", HELLO_DIR);
		return -ENOMEM;
	}
	
	// 在目录 /proc/hello_dir 中创建文件 hello
	if (!proc_create(HELLO_FILE, 0666, hello_dir, &hello_fops)) {
		printk(KERN_ERR "Failed to create /proc/%s/%s file\n", HELLO_DIR, HELLO_FILE);
		remove_proc_entry(HELLO_DIR, NULL);
		return -ENOMEM;
	}
	
	printk(KERN_INFO "Hello Module4\n");
	return 0;
}

static void __exit my_module_exit(void) {
	remove_proc_entry(HELLO_FILE, NULL);
	remove_proc_entry(HELLO_DIR, NULL);
	printk(KERN_INFO "GoodBye Module4\n");
}

module_init(my_module_init);
module_exit(my_module_exit);
MODULE_LICENSE("GPL");
