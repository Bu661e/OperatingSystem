#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/workqueue.h>
#include <linux/time.h>

#define DELAY_SECONDS 10
#define TIMER_INTERVAL (5 * HZ)

static struct workqueue_struct *my_workqueue;
static struct work_struct my_work;
static struct delayed_work my_delayed_work;
static struct timer_list my_timer;

struct timeval tv;
struct tm tm;


// 立即执行的工作项处理函数
static void my_work_handler(struct work_struct *work)
{
        do_gettimeofday(&tv);
    time_to_tm(tv.tv_sec, 0, &tm);
    printk(KERN_INFO "正在执行【立即执行】的任务 当前时间： %ld-%d-%d %d:%d:%d\n" ,          
            (long)tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
           tm.tm_hour, tm.tm_min, tm.tm_sec);
}

// 延迟执行的工作项处理函数
static void my_delayed_work_handler(struct work_struct *work)
{
    do_gettimeofday(&tv);
    time_to_tm(tv.tv_sec, 0, &tm);
    printk(KERN_INFO "正在执行【延迟执行】的任务 当前时间： %ld-%d-%d %d:%d:%d\n"  ,         
            (long)tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
           tm.tm_hour, tm.tm_min, tm.tm_sec);
}

// 定时器处理函数
static void my_timer_handler(struct timer_list *timer)
{


    do_gettimeofday(&tv);
    time_to_tm(tv.tv_sec, 0, &tm);
    printk(KERN_INFO "当前时间： %ld-%d-%d %d:%d:%d\n",
           (long)tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
           tm.tm_hour, tm.tm_min, tm.tm_sec);
    mod_timer(&my_timer, jiffies + TIMER_INTERVAL);
}

// 模块加载函数
static int __init workqueue_test_init(void)
{


        // 初始化定时器
    timer_setup(&my_timer, my_timer_handler, 0);
    mod_timer(&my_timer, jiffies + TIMER_INTERVAL);

    // 创建工作队列
    my_workqueue = create_workqueue("my_workqueue");
    if (!my_workqueue) {
        printk(KERN_ERR "无法创建工作队列\n");
        return -ENOMEM;
    }

    // 初始化并排队立即执行的工作项
    INIT_WORK(&my_work, my_work_handler);
    queue_work(my_workqueue, &my_work);

    // 初始化并排队延迟执行的工作项
    INIT_DELAYED_WORK(&my_delayed_work, my_delayed_work_handler);
    queue_delayed_work(my_workqueue, &my_delayed_work, DELAY_SECONDS * HZ);



    return 0;
}

// 模块卸载函数
static void __exit workqueue_test_exit(void)
{
    // 停止延迟执行的工作项
    cancel_delayed_work(&my_delayed_work);

    // 停止定时器
    del_timer(&my_timer);

    // 刷新和销毁工作队列
    flush_workqueue(my_workqueue);
    destroy_workqueue(my_workqueue);

    printk(KERN_INFO "卸载工作队列测试模块\n");
}

module_init(workqueue_test_init);
module_exit(workqueue_test_exit);

MODULE_LICENSE("GPL");

