// #include <linux/init.h>
// #include <linux/module.h>
// #include <linux/kernel.h>
// #include <linux/interrupt.h>

// static int irq_num = 0;
// static char *devname = NULL;
// static void *dev = NULL;
// static int irq_count = 0;
// int req_ret = 0;
// static spinlock_t irq_count_lock;
// module_param(irq_num, int, S_IRUGO);
// module_param(devname, charp, S_IRUGO);
// // 中断处理函数
// static irqreturn_t my_interrupt_handler(int irq, void *dev_id)
// {
//     spin_lock(&irq_count_lock); // 获取自旋锁
//     irq_count++;
//     printk(KERN_INFO "Interrupt %d occurred. Count: %d\n", irq, irq_count);
//     spin_unlock(&irq_count_lock); // 释放自旋锁
//     return IRQ_HANDLED;
// }

// static int __init interrupt_module_init(void)
// {
//     // 初始化自旋锁
//     spin_lock_init(&irq_count_lock);
    
//     // 检查参数
//     if (!irq_num || !devname) {
//         printk(KERN_ERR "Missing parameters\n");
//         return -EINVAL;
//     }

//     printk(KERN_INFO "Modules starts...\n");

    
//     // 注册中断处理函数
//     req_ret = request_irq(irq_num, my_interrupt_handler, IRQF_SHARED, devname, (void *)&dev);
//     printk(KERN_INFO "req_ret is %d...\n", req_ret);
//     if (req_ret)
//     {
//         printk(KERN_ERR "Failed to request IRQ:%d for device %s\n", irq_num, devname);
//         return -EFAULT;
//     }

//     printk(KERN_INFO "tasklet_dev request IRQ:%d success...\n", irq_num);
//     return 0;
// }

// static void __exit interrupt_module_exit(void)
// {
//     // 卸载中断处理函数
//     free_irq(irq_num, (void *)&dev);
//     printk(KERN_INFO "Modules exits...\n");
//     printk(KERN_INFO "tasklet_dev request IRQ:%d leaving success...\n", irq_num);
// }

// module_init(interrupt_module_init);
// module_exit(interrupt_module_exit);



// MODULE_LICENSE("GPL");
// MODULE_AUTHOR("Your Name");

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>

static int irq_num = 0;
static char *devname = NULL;
static int irq_count = 0;
int req_ret = 0;
static struct tasklet_struct my_tasklet;
DEFINE_SPINLOCK(irq_count_lock);

module_param(irq_num, int, S_IRUGO);
module_param(devname, charp, S_IRUGO);

// Tasklet 处理函数
static void my_tasklet_handler(unsigned long data)
{
    spin_lock(&irq_count_lock); // 获取自旋锁
    irq_count++;
    printk(KERN_INFO "Tasklet: Interrupt occurred. Count: %d\n", irq_count);
    spin_unlock(&irq_count_lock); // 释放自旋锁
}

// 中断处理函数
static irqreturn_t my_interrupt_handler(int irq, void *dev_id)
{
    // 调度 tasklet
    tasklet_schedule(&my_tasklet);
    return IRQ_HANDLED;
}

static int __init interrupt_module_init(void)
{
    // 初始化自旋锁
    spin_lock_init(&irq_count_lock);

    // 检查参数
    if (!irq_num || !devname) {
        printk(KERN_ERR "Missing parameters\n");
        return -EINVAL;
    }

    printk(KERN_INFO "Module starts...\n");

    // 初始化 tasklet
    tasklet_init(&my_tasklet, my_tasklet_handler, 0);

    // 注册中断处理函数
    req_ret = request_irq(irq_num, my_interrupt_handler, IRQF_SHARED, devname, (void *)&irq_num);
    printk(KERN_INFO "req_ret is %d...\n", req_ret);
    if (req_ret)
    {
        printk(KERN_ERR "Failed to request IRQ:%d for device %s\n", irq_num, devname);
        return -EFAULT;
    }

    printk(KERN_INFO "Request IRQ:%d success...\n", irq_num);
    return 0;
}

static void __exit interrupt_module_exit(void)
{
    // 卸载中断处理函数
    free_irq(irq_num, (void *)&irq_num);

    // 清理 tasklet
    tasklet_kill(&my_tasklet);

    printk(KERN_INFO "Module exits...\n");
}

module_init(interrupt_module_init);
module_exit(interrupt_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
