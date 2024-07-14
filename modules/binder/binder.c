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
#include "asm/uaccess.h"
#include "linux/errno.h"
#include "linux/slab.h"
#include "binder.h"
#include "log_define.h"
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
    wait_queue_head_t wait;
};

struct binder_thread {
	struct binder_proc *proc;
	struct list_head thread_node;
	struct list_head todo;
    wait_queue_head_t wait;
    struct task_struct *task;
    struct mutex lock;
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
	unsigned int cmd;
    struct binder_proc *from_proc;
    struct binder_thread *from_thread;
    struct binder_proc *to_proc;
    struct binder_thread *to_thread;
    struct binder_buffer buffer;
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

// 如果一个进程多次open？ 上层保证单例还是底层返回掉？
// 如果要在驱动中保证单例的话，需要根据task_struct的tgid字段判断
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
    init_waitqueue_head(&proc->wait);

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

    // 对于一般的服务进程而言，既然走到了这里基本上是异常退出，操作系统进行资源回收
    // 几乎不会有服务主动关闭binder的场景，所以只要保证内存不泄露就行了，回收得可以粗暴一点

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

    // 释放todo链表里的消息，但是目前还没有加入todo消息，先不管
    mutex_unlock(&proc->lock);

    // 如果是服务，则释放服务相关的资源
    if (proc->service) {
        binder_free_service(proc->service);
    }

    kfree(proc);
    return 0;
}

static int do_get_service_id(const char *name)
{
    struct binder_service *service;
	struct rb_node *n;

    // 这里是通过name查询服务的，获取到id，性能较低，之后使用服务时通过id查询服务，就能发挥红黑树的优势
    for (n = rb_first(&binder_services.rb_root); n != NULL; n = rb_next(n)) {
		service = rb_entry(n, struct binder_service, rb_node);
        if (!strcmp(service->name, name)) {
            return service->id;
        }
	}

    return -1;
}

static int binder_get_service_id(unsigned long arg)
{
	void __user *ubuf = (void __user *)arg;
    struct binder_get_service_arg args;

    if (copy_from_user(&args, ubuf, sizeof(args))) {
        LOGE("Can't copy from user\n");
        return -EFAULT;
    }

    return do_get_service_id(args.name);
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

    return  id < BINDER_MAX_SERVICE_NUM? (id + 1) : -1;
}

static int do_add_service(struct binder_proc *proc, struct binder_add_service_arg *service_arg)
{
    struct binder_service *service;
    struct rb_node **p;
    struct rb_node *parent;
    struct binder_service *ser;



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

    LOGD("Add service %s\n", service->name);
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

    if (binder_services.count >= BINDER_MAX_SERVICE_NUM) {
        LOGE("binder only support %d services\n", BINDER_MAX_SERVICE_NUM);
        return -EFAULT;
    }

    if (proc->service) {
        LOGE("Only one service can be added\n");
        return -EFAULT;
    }

    if (do_get_service_id(ser.name) >= 0) {
        LOGE("Service has been added\n");
        return -EFAULT;
    }

    return do_add_service(proc, &ser);
}

static int binder_enter_loop(struct binder_proc *proc, struct binder_thread *thread)
{
    if (!proc->service) {
        LOGE("Only service can enter loop\n");
        return -EFAULT;
    }

    if (proc->service->looper) {
        LOGE("One thread has entered loop, binder only support single thread service\n");
        return -EFAULT;
    }

    proc->service->looper = thread;
    return 0;
}

static struct binder_thread *binder_get_thread(struct binder_proc *proc)
{
    struct binder_thread *thread;

    list_for_each_entry(thread, &proc->threads, thread_node) {
        if (thread->task->pid == current->pid) {
            return thread;
        }
    }

    // 不存在，则创建
    thread = kzalloc(sizeof(struct binder_thread), GFP_KERNEL);
    if (thread == NULL) {
        LOGE("Can't create thread\n");
        return NULL;
    }

    thread->proc = proc;
    thread->task = current;
    INIT_LIST_HEAD(&thread->todo);
    thread->transaction_stack = NULL;
    mutex_init(&thread->lock);
    init_waitqueue_head(&thread->wait);

    mutex_lock(&proc->lock);
    list_add(&thread->thread_node, &proc->threads);
    mutex_unlock(&proc->lock);

    return thread;
}

static struct binder_service *binder_get_service(int id)
{
	struct rb_node *node = binder_services.rb_root.rb_node;

	while (node)
	{
		struct binder_service *service = container_of(node, struct binder_service, rb_node);
        if (id == service->id) {
            return service;
        } else if (id < service->id) {
            node = node->rb_left;
        } else {
            node = node->rb_right;
        }
	}

	return NULL;
}

static struct binder_proc *binder_get_service_proc(struct binder_service *service)
{
    if (!service) {
        LOGE("Service is NULL\n");
        return NULL;
    }

    return service->proc;
}

#if 0
static int binder_is_service(struct binder_proc *proc)
{
    return proc->service != NULL;
}
#endif

static int do_thread_read(struct binder_proc *proc,
                        struct binder_thread *thread,
                        struct binder_transaction_data *in,
                        struct binder_transaction_data *out)
{
    struct binder_transaction *transaction;
	void __user *ubuf = (void __user *)in->buffer;
    int ret;
    struct list_head *todo;
    struct mutex *lock;
    wait_queue_head_t *wait;

    switch(in->cmd) {
    // 服务端接收请求
    case BINDER_TRANSACTION:
        lock = &proc->lock;
        todo = &proc->todo;
        wait = &proc->wait;
        break;

    // 客户端接收回复
    case BINDER_REPLY:
        lock = &thread->lock;
        todo = &thread->todo;
        wait = &thread->wait;
        break;

    default:
        LOGE("Unsupport read cmd\n");
        return -EFAULT;
    }

    // 无限循环等待数据，暂不支持非阻塞
    for(;;) {
		if (wait_event_interruptible(*wait, !list_empty(todo)) != 0) {
            // 被信号中断
            return -ERESTARTSYS;
        }
        // wait_event退出的时候实际已经满足了todo非空的条件
        // 但是可能在多线程的时候todo数据又被别的线程拿走
        mutex_lock(lock);
        if (!list_empty(todo))
        {
            transaction = list_first_entry(todo, struct binder_transaction, list);
            list_del(&transaction->list);
            mutex_unlock(lock);
            break;
        }
        mutex_unlock(lock);
    }

    if (transaction->buffer.size > in->size) {
        LOGE("Read data size larger than buffer size\n");
        ret = -ENOMEM;
        goto exit;
    }

    // 拿到数据, 拷贝给用户空间，这是第二次拷贝
    if (copy_to_user(ubuf, transaction->buffer.buffer, transaction->buffer.size))
    {
        LOGE("Can't copy to user\n");
        ret = -EFAULT;
        goto exit;
    }

    out->cmd = transaction->cmd;
    out->id = -1; // 读的时候不需要id
    out->buffer = in->buffer; // 缓冲区不变的，始终指向自己进程所准备的缓冲区
    out->size = transaction->buffer.size;
    out->code = transaction->code;

    if (in->cmd == BINDER_TRANSACTION) {
        // 服务端读请求，等会儿还要回复，要知道回复给谁，所以存一下transaction
        thread->transaction_stack = transaction;
    }
    else {
        // 客户端读回复，读到的就没用了，释放掉
        thread->transaction_stack = NULL;
        kfree(transaction);
    }

    return 0;

exit:
    kfree(transaction);
    return ret;
}

static int binder_thread_read(struct binder_proc *proc,
                                struct binder_thread *thread,
                                unsigned long buffer)
{
    struct binder_transaction_data in;
    struct binder_transaction_data out;
    int ret;
	void __user *ubuf = (void __user *)buffer;


    // 先把用户空间的参数传给in
    if (copy_from_user(&in, ubuf, sizeof(struct binder_transaction_data))) {
        LOGE("Can't copy from user\n");
        return -EINVAL;
    }

    ret = do_thread_read(proc, thread, &in, &out);
    if (ret < 0) {
        LOGE("Thread read failed, ret: %d\n", ret);
        return ret;
    }

    // 把接收到的数据还给用户空间，这里的缓存区是复用
    if (copy_to_user(ubuf, &out, sizeof(struct binder_transaction_data))) {
        LOGE("Copy to user failed\n");
        return -EINVAL;
    }

    return 0;
}


static int do_thread_write(struct binder_proc *proc,
                            struct binder_thread *thread,
                            struct binder_transaction_data *data)
{
    // 创建一个transaction对象，推入目标的todo队列，然后唤醒目标
    struct binder_transaction *transaction;
    struct binder_service *service;
	void __user *ubuf = (void __user *)data->buffer;
    int ret;

    transaction = (struct binder_transaction*)kzalloc(sizeof(*transaction), GFP_KERNEL);
    if (!transaction) {
        LOGE("Can't alloc memory for transaction\n");
        return -ENOMEM;
    }

    if (data->size == 0) {
        // 相当于只传了个code，没有参数
        transaction->buffer.buffer = NULL;
    } else {
        transaction->buffer.buffer = kzalloc(data->size, GFP_KERNEL);
        if (!transaction->buffer.buffer) {
            LOGE("Can't alloc memory for buffer\n");
            ret = -ENOMEM;
            goto alloc_buffer_err;
        }

        // 把数据拷贝到内核，这是第一次拷贝
        if (copy_from_user(transaction->buffer.buffer, ubuf, data->size)) {
            LOGE("Can't copy from user\n");
            ret = -EFAULT;
            goto cpy_from_user_err;
        }
    }

    transaction->cmd = data->cmd;
    transaction->buffer.size = data->size;
    transaction->code = data->code;
    transaction->from_proc = proc;
    transaction->from_thread = thread;

    switch (data->cmd) {
    // 客户端发起请求
    case BINDER_TRANSACTION:
        // client发给service时，消息挂在进程下，service可以多线程处理消息
        service = binder_get_service(data->id);
        transaction->to_proc = binder_get_service_proc(service);
        transaction->to_thread = NULL;

        // 如果service为空，那这里to_proc肯定也是空，所以只需判断to_proc
        if (transaction->to_proc == NULL) {
            LOGE("No transaction target\n");
            ret = -ENODEV;
            goto exit;
        }

        // 把数据放进目标队列
        mutex_lock(&transaction->to_proc->lock);
        list_add(&transaction->list, &transaction->to_proc->todo);
        mutex_unlock(&transaction->to_proc->lock);
        // 唤醒服务进程下所有的looper
	    wake_up_interruptible(&transaction->to_proc->wait);
        break;

    // 服务端回复
    case BINDER_REPLY:
        if (thread->transaction_stack) {
            // 取出先前记录好的传输，找到给谁回复
            // service回复client时，必须要回复给正确的线程
            transaction->to_proc = thread->transaction_stack->from_proc;
            transaction->to_thread = thread->transaction_stack->from_thread;
            // transaction_stack 的作用就是这里，用完了就释放掉
            kfree(thread->transaction_stack);
            if (transaction->to_proc == NULL || transaction->to_thread == NULL)
            {
                LOGE("No reply target\n");
                ret = -EFAULT;
                goto exit;
            }
        } else {
            // 如果没有transaction_stack，其实是没法办回复的，甚至给了应用层一个制造错误的漏洞
            LOGE("No transaction stack\n");
            ret = -EFAULT;
            goto exit;
        }
        // 把数据放进目标队列
        mutex_lock(&transaction->to_thread->lock);
        list_add(&transaction->list, &transaction->to_thread->todo);
        mutex_unlock(&transaction->to_thread->lock);
        // 只需要唤醒回复的线程即可
	    wake_up_interruptible(&transaction->to_thread->wait);
        break;

    default:
        LOGE("Unsupport binder data cmd\n");
        ret = -EFAULT;
        goto exit;
    }

    return 0;

exit:
cpy_from_user_err:
    kfree(transaction->buffer.buffer);
alloc_buffer_err:
    kfree(transaction);
    return ret;
}

/* proc&thread: sender's proc and threaa
 * buffer: the buffer in user space, the type should be struct binder_transaction_data
 * bufer size: the buffer size, should be sizeof(struct binder_transaction_data)
 */
static int binder_thread_write(struct binder_proc *proc,
                                struct binder_thread *thread,
                                unsigned long buffer)
{
    struct binder_transaction_data data;
	void __user *ubuf = (void __user *)buffer;

    if (copy_from_user(&data, ubuf, sizeof(struct binder_transaction_data))) {
        LOGE("Can't copy from user\n");
        return -EINVAL;
    }

    return do_thread_write(proc, thread, &data);
}


static int binder_write_read(struct binder_proc *proc,
                            struct binder_thread *thread,
                            unsigned long arg)
{
	void __user *ubuf = (void __user *)arg;
	struct binder_write_read bwr;
    int ret;

	if (copy_from_user(&bwr, ubuf, sizeof(bwr))) {
		return -EFAULT;
	}

	if (bwr.write_buffer) {
        LOGE("write pid:%d\n", current->pid);
		ret = binder_thread_write(proc, thread, bwr.write_buffer);
        if (ret < 0) {
            return ret;
        }
	}

	if (bwr.read_buffer) {
        LOGE("read pid:%d\n", current->pid);
		ret = binder_thread_read(proc, thread, bwr.read_buffer);
        if (ret < 0) {
            return ret;
        }
	}

    return 0;
}

static long binder_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    unsigned int size = _IOC_SIZE(cmd);
    struct binder_proc *proc = (struct binder_proc*)filp->private_data;
    struct binder_thread *thread;

    if (proc == NULL) {
        LOGE("proc is NULL\n");
        return -EFAULT;
    }

    thread = binder_get_thread(proc);
    if (!thread) {
        LOGE("Can't get thread\n");
        return -EFAULT;
    }

    switch (cmd) {
    case BINDER_WRITE_READ:
        if (size != sizeof(struct binder_write_read)) {
            LOGE("Add service arg size err");
            return -EINVAL;
        }
        return binder_write_read(proc, thread, arg);

    case BINDER_GET_SERVICE:
        if (size != sizeof(struct binder_get_service_arg)) {
            LOGE("Add service arg size err");
            return -EINVAL;
        }
        return binder_get_service_id(arg);

    case BINDER_ADD_SERVICE:
        if (size != sizeof(struct binder_add_service_arg)) {
            LOGE("Add service arg size err");
            return -EINVAL;
        }
        return binder_add_service(proc, arg);

    case BINDER_ENTER_LOOP:
        return binder_enter_loop(proc, thread);

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
