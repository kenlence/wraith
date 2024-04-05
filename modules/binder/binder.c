#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/mutex.h>
#include <linux/file.h>
#include <linux/list.h>
#include <linux/string.h>
#include "../log_define.h"
#include "./binder.h"
#include <linux/sizes.h>
#include <linux/rbtree.h>

#define BINDER_MAX_SERVICE_NUM 1024

struct binder_service;

/* 每一个进程打开/dev/binder都会为其构造一个binder_proc实例，
之后的binder操作均以binder_proc对象为基础 */
struct binder_proc {
	struct list_head proc_node;
    struct list_head threads;
    struct list_head todo;
    struct mutex lock;
    // 如果是服务，指向服务结构体，如果不是则为NULL
    struct binder_service *service;
};

struct binder_thread {
	struct binder_proc *proc;
	struct list_head thread_node;
	struct list_head todo;
    struct list_head looper;
    struct task_struct *task;
	wait_queue_head_t wait;
    //临时存储传输过来的信息，回复的时候用
	struct binder_transaction *transaction_stack;
};

//用于描述一个服务
// 一个进程可以是一个服务
// 一个线程可以是一个looper
struct binder_service {
    char name[BINDER_SERVICE_NAME_MAX_LEN];
    struct rb_node rb_node;
    struct binder_proc *proc;
    struct binder_thread *looper;
    int id;
};

struct binder_services {
    struct rb_root rb_root;
    int count;
    struct mutex lock;
};

struct binder_buffer {
    void *buffer;
    unsigned int size;
};

//内核中推入目标todo队列的对象
struct binder_transaction {
    struct binder_proc *from_proc;
    struct binder_thread *from_thread;
    struct binder_proc *to_proc;
    struct binder_thread *to_thread;
    struct binder_buffer *buffer;
    unsigned int code;
    struct list_head list;
};

//用于管理所有的服务，以红黑树的方式组织
static struct binder_services binder_services;
//用于分配id
//使用链表管理所有的proc
static LIST_HEAD(binder_procs);
//全局锁
static DEFINE_MUTEX(binder_global_lock);

static void binder_lock(void)
{
	mutex_lock(&binder_global_lock);
}

static void binder_unlock(void)
{
	mutex_unlock(&binder_global_lock);
}

static int binder_open(struct inode *nodp, struct file *filp)
{
    struct binder_proc *proc;

    proc = kzalloc(sizeof(struct binder_proc), GFP_KERNEL);
    if (proc == NULL) {
        LOGE("Can't alloc memory for bind_proc\n");
        return -ENOMEM;
    }

    // 初始化 proc中的成员
    INIT_LIST_HEAD(&proc->threads);
    INIT_LIST_HEAD(&proc->todo);
    mutex_init(&proc->lock);
    proc->service = NULL;

	binder_lock();
    //把申请的proc添加到全局管理
    list_add(&proc->proc_node, &binder_procs);
	binder_unlock();

    filp->private_data = proc;
    return 0;
}

static void binder_free_service(struct binder_service *service)
{
    //将服务从全局删除
    mutex_lock(&binder_services.lock);
    rb_erase(&service->rb_node, &binder_services.rb_root);
    mutex_unlock(&binder_services.lock);

    kfree(service);
}

static int binder_release(struct inode *nodp, struct file *filp)
{
    //TODO:优化项：异步释放。把需要释放的proc推到工作队列, 稍后再完成。
    //目前实现的版本是同步释放的，可能会带来一些系统卡顿。
    struct binder_proc *proc = (struct binder_proc*)filp->private_data;

    if (!proc) {
        return 0;
    }

    binder_lock();
    //将proc从全局中移除
	list_del(&proc->proc_node);
    binder_unlock();

    //释放proc下面管理的资源
    mutex_lock(&proc->lock);
    //注意此时可能会有一些线程正在等待，需要通知他们释放

    // 释放所有的线程资源，但是目前还没有加入线程，先不管

    // 释放todo链表里的消息，但是目前还没有加入todo消息，先不管
    mutex_unlock(&proc->lock);

    // 如果是服务，则释放服务相关的资源
    if (proc->service) {
        binder_free_service(proc->service);
    }

    kfree(proc);
    return 0;
}

static int binder_alloc_service_id(void)
{
    int id = 0;
	struct rb_node *n;
    struct binder_service *service;

    for (n = rb_first(&binder_services.rb_root); n != NULL; n = rb_next(n)) {
		service = rb_entry(n, struct binder_service, rb_node);
        if (service->id > id + 1) {
            break;
        }
        id = service->id;
	}

    if (id < BINDER_MAX_SERVICE_NUM) {
        return id + 1;
    } else {
        return -1;
    }
}

static int do_add_service(struct binder_proc *proc, struct binder_add_service_arg *service_arg)
{
    struct binder_service *service;
    struct rb_node **p;
    struct rb_node *parent;
    struct binder_service *ser;

    if (binder_services.count >= BINDER_MAX_SERVICE_NUM) {
        LOGE("binder only support %d services\n", BINDER_MAX_SERVICE_NUM);
        return -EFAULT;
    }

    service = (struct binder_service*)kzalloc(sizeof(*service), GFP_KERNEL);
    if (!service) {
        LOGE("Can't alloc memory\n");
        return -ENOMEM;
    }

    //填充节点
    strcpy(service->name, service_arg->name);
    service->proc = proc;
    service->id = binder_alloc_service_id();
    if (service->id < 0) {
        LOGE("Can't alloc service id\n");
        goto exit;
    }

    mutex_lock(&binder_services.lock);
    //把节点推进红黑树
    p = &binder_services.rb_root.rb_node;
    parent = NULL;

    while (*p) {
        parent = *p;
        ser = rb_entry(parent, struct binder_service, rb_node);

        if (service->id == ser->id) {
            LOGE("Service id error\n");
            mutex_unlock(&binder_services.lock);
            goto exit;
        }

        if (service->id < ser->id) {
            p = &parent->rb_left;
        } else {
            p = &parent->rb_right;
        }
    }

    rb_link_node(&service->rb_node, parent, p);
    rb_insert_color(&service->rb_node, &binder_services.rb_root);

    binder_services.count++;
    mutex_unlock(&binder_services.lock);
    proc->service = service;

    return 0;

exit:
    kfree(service);
    return -EFAULT;
}

static int binder_add_service(struct binder_proc *proc, unsigned long arg)
{
    struct binder_add_service_arg ser;
	void __user *ubuf = (void __user *)arg;

    if (copy_from_user(&ser, ubuf, sizeof(ser))) {
        LOGE("Can't copy from user\n");
		return -EFAULT;
	}

    if (ser.name == NULL) {
        LOGE("service arg error\n");
        return -EINVAL;
    }

    return do_add_service(proc, &ser);
}

static long binder_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    unsigned int size = _IOC_SIZE(cmd);
    struct binder_proc *proc = (struct binder_proc*)filp->private_data;
    //struct binder_thread *thread;

    if (proc == NULL) {
        LOGE("proc is NULL\n");
        return -EFAULT;
    }

#if 0
    // 获取当前线程的对象，如果不存在则创建一个
    thread = binder_get_thread(proc);
    if (!thread) {
        LOGE("Can't get thread\n");
        return -EFAULT;
    }
#endif

    switch (cmd) {
#if 0
    case BINDER_WRITE_READ:
        if (size != sizeof(struct binder_write_read)) {
            LOGE("Add service arg size err");
            return -EINVAL;
        }
        return binder_write_read(arg);
    case BINDER_GET_SERVICE:
        if (size != sizeof(struct binder_get_service_arg)) {
            LOGE("Add service arg size err");
            return -EINVAL;
        }
        return binder_get_service(arg);
#endif

    case BINDER_ADD_SERVICE:
        if (size != sizeof(struct binder_add_service_arg)) {
            LOGE("Add service arg size err");
            return -EINVAL;
        }
        return binder_add_service(proc, arg);
#if 0
    case BINDER_ENTER_LOOP:
        return binder_enter_loop(proc, thread);
#endif
    case BINDER_DEL_SERVICE:
    default:
        LOGW("Unsupport binder ioctl cmd: %d\n", cmd);
        break;
    }

    return 0;
}

//read 和 write接口只用于调试，实际功能由IOCTL实现
//read 打印出全部的服务
#define READ_BUF_MAX_SIZE 1024

ssize_t binder_read(struct file *filp, char __user *buffer, size_t size , loff_t *ppos)
{
    int ret;
    char *kbuf;
    int len = 0;
	struct rb_node *n;
    struct binder_service *service;

    kbuf = (char*)kmalloc(READ_BUF_MAX_SIZE, GFP_KERNEL);
    if (!kbuf) {
        LOGE("Can't alloc memory");
        return 0;
    }

    len += snprintf(kbuf, READ_BUF_MAX_SIZE - len, "%-*s%-*s\n", 8, "id", 16, "name");
    for (n = rb_first(&binder_services.rb_root); n != NULL; n = rb_next(n)) {
		service = rb_entry(n, struct binder_service, rb_node);
        len += snprintf(kbuf + len, READ_BUF_MAX_SIZE - len, "%-*d%-*s\n", 8, service->id, 16, service->name);
	}

    ret = simple_read_from_buffer(buffer, size, ppos, kbuf, len);
    kfree(kbuf);

    return ret;
}

static ssize_t binder_write(struct file *filp, const char __user *buffer, size_t count, loff_t *ppos)
{
    return count;
}

static void binder_services_init(void)
{
    binder_services.rb_root = RB_ROOT;
    binder_services.count = 0;
    mutex_init(&binder_services.lock);
}

static struct file_operations binder_fops = {
	.owner = THIS_MODULE,
    .write = binder_write,
    .read = binder_read,
	.unlocked_ioctl = binder_ioctl,
	.compat_ioctl = binder_ioctl,
	.open = binder_open,
	.release = binder_release,
};

static struct miscdevice binder_miscdev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "binder",
    .fops = &binder_fops,
};

static int __init binder_init(void)
{
    int ret;

    binder_services_init();

    ret = misc_register(&binder_miscdev);
    if (ret < 0) {
        LOGE("register binder misc device failed\n");
        return -EFAULT;
    }

    return 0;
}
module_init(binder_init);

static void __exit binder_exit(void)
{
    LOGW("binder exit\n");
    LOGE("binder can't exit corrextly, must reboot system\n");
    misc_deregister(&binder_miscdev);
}
module_exit(binder_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Lucas");
