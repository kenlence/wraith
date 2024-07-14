/* Binder test cases
 *
 */

#include <iostream>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "modules/binder.h"
#include <unistd.h>
#include "string.h"
#include <string>
#include <stdio.h>

#define BINDER_NAME "/dev/binder"
#define SERVICE_NAME "binder_tester"

#define TAG "BinderServiceTester"

#define LOG(fmt, ...) \
    printf("[%s][%s-%d]" fmt"", TAG, __FUNCTION__, __LINE__, ##__VA_ARGS__)

class ServiceRequest {
public:
};

// service
class Service {
public:
    Service(const std::string &name);
    Service(const char *name);
    ~Service();
    int getRequest(int size);
private:
    int fd;
    void addService();
    std::string name;
};

Service::Service(const std::string &name) {
    this->name = name;
    fd = open(BINDER_NAME, O_RDWR);
    if (fd < 0) {
        LOG("Can't open /dev/binder");
        throw "open binder fail";
    }
    addService();
}

Service::Service(const char *name)
{
    this->name = std::string(name);
    fd = open(BINDER_NAME, O_RDWR);
    if (fd < 0) {
        LOG("Can't open /dev/binder");
        throw "open binder fail";
    }
    addService();
}

Service::~Service() {
    close(fd);
}

void Service::addService() {
    struct binder_add_service_arg arg;

    strncpy(arg.name, name.c_str(), BINDER_SERVICE_NAME_MAX_LEN);
    if(ioctl(fd, BINDER_ADD_SERVICE, &arg)) {
        throw "Can't add service";
    }
}

int Service::getRequest(int size) {
    struct binder_transaction_data data;
    struct binder_transaction_data w;
    void *buffer = malloc(size);
    int ret;
    struct binder_write_read wr = {
        .write_buffer = 0,
        .read_buffer = (unsigned long)&data,
    };
    char reply[256];

    if (!buffer) {
        LOG("Can't malloc buffer");
        return -1;
    }

    data.cmd = BINDER_TRANSACTION;
    data.id = 0;
    data.code = 0;
    data.buffer = buffer;
    data.size = size;


    ret = ioctl(fd, BINDER_WRITE_READ, &wr);
    if (ret < 0) {
        LOG("Can't write read binder");
        return -1;
    }

    LOG("read return success\n");
    LOG("cmd: %d\n", data.cmd);
    LOG("id: %d\n", data.id);
    LOG("code: %d\n", data.code);
    LOG("buffer char: %s\n", (char *)buffer);

    wr.write_buffer = (unsigned long)&w;
    wr.read_buffer = 0;

    w.cmd = BINDER_REPLY;
    w.id = 0;
    w.code = data.code;
    sprintf(reply, "reply code: %d, data:%s ", data.code, (char *)data.buffer);
    w.buffer = reply;
    w.size = strlen(reply) + 1;

    LOG("Reply: %s\n", reply);
    ret = ioctl(fd, BINDER_WRITE_READ, &wr);
    if (ret < 0) {
        LOG("Can't write read binder");
        return -1;
    }

    free(buffer);
    return 0;
}

class ServiceTester: public Service {
public:
    ServiceTester();
private:
};

int main(int argc, char* argv[])
{
    try {
        auto service = new Service("testService");
        for(;;)
        {
            service->getRequest(1024);
            LOG("Return from get request\n");
            sleep(1);
        }
    } catch (const char *msg) {
        LOG("%s", msg);
        return -1;
    }

    return 0;
}