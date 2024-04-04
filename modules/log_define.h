#include "linux/printk.h"

#define LOGE(fmt, ...) \
    printk(KERN_ERR "[Error][%s-%s-%d]" fmt"", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define LOGW(fmt, ...) \
    printk(KERN_WARNING "[Warning][%s-%s-%d]" fmt"", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define LOGD(fmt, ...) \
    printk(KERN_DEBUG "[Debug][%s-%s-%d]" fmt"", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
