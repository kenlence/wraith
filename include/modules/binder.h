#ifndef __BINDER_H_
#define __BINDER_H_

#include <linux/ioctl.h>
#include <linux/types.h>

#define BINDER_SERVICE_NAME_MAX_LEN 32

struct binder_write_read {
	unsigned long write_buffer;
	unsigned long read_buffer;
};

struct binder_add_service_arg {
	char name[BINDER_SERVICE_NAME_MAX_LEN];
};

struct binder_get_service_arg {
	char name[BINDER_SERVICE_NAME_MAX_LEN];
};

// 应用层使用ioctl write read时传入的参数结构体，描述传输信息
// struct binder_write_read 的buffer指的就是这儿的binder_transaction_data
// 在读的时候，这个结构体的内存空间是复用的，首先用户空间要通过该结构体告诉内核该怎么读
// 内核读到数据之后，要用读到的数据覆盖写该结构体中的数据
struct binder_transaction_data {
	// 写的时候：客户端发送请求： BINDER_TRANSACTION, 服务端回复： BINDER_REPLY
	// 读的时候：客户端读回复：BINDER_REPLY, 服务端读请求：BINDER_TRANSACTION
	unsigned int cmd;
	unsigned int id; //如果是客户端发送，id用于指明发送的目标
						//如果是服务端回复，那id可以为空，发送的目标从传输栈中取出
	//客户端发送给服务端的命令码，服务端回复时也需要，客户端进行校验确认答为所问
	unsigned int code;
    void *buffer;
    unsigned int size;
};

#define BINDER_REPLY		0
#define BINDER_TRANSACTION 	1

//ioctl cmd define
#define BINDER_WRITE_READ		_IOWR('b', 1, struct binder_write_read)
#define BINDER_GET_SERVICE		_IOW('b', 2, struct binder_get_service_arg)
#define BINDER_ADD_SERVICE		_IOW('b', 3, struct binder_add_service_arg)
#define BINDER_ENTER_LOOP		_IOW('b', 4, long)

#endif