#include "linux/printk.h"

#define LOGE(fmt, ...) \
    printk(KERN_ERR "[Error][%s-%d]" fmt"", __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define LOGW(fmt, ...) \
    printk(KERN_WARNING "[Warning][%s-%d]" fmt"", __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define LOGD(fmt, ...) \
    printk(KERN_DEBUG "[Debug][%s-%d]" fmt"", __FUNCTION__, __LINE__, ##__VA_ARGS__)
