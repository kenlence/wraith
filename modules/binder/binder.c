#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>

static int __init binder_init(void)
{
    printk("binder load\n");
}

module_init(binder_init);
