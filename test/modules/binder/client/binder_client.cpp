/* Binder test cases
 *
 */

#include <sys/ioctl.h>
#include <fcntl.h>
#include "modules/binder.h"
#include <unistd.h>
#include <string>
#include <string.h>

#define BINDER_NAME "/dev/binder"
#define SERVICE_NAME "binder_tester"

#define TAG "BinderClientTester"

#define LOG(fmt, ...) \
    printf("[%s][%s-%d]" fmt"", TAG, __FUNCTION__, __LINE__, ##__VA_ARGS__)


// A Proxy is always mapped to a service.
class Proxy{
public:
    Proxy(const std::string &name);
    Proxy(const char *name);
    ~Proxy();
    int sendMessage(unsigned char *buf, int size);
private:
    int fd;
    int id;
    void getService();
    std::string name;
};

Proxy::Proxy(const std::string &name)
{
    this->name = name;
    fd = open(BINDER_NAME, O_RDWR);
    if (fd < 0) {
        LOG("Can't open /dev/binder\n");
    }
    getService();
}

Proxy::Proxy(const char *name)
{
    this->name = std::string(name);
    fd = open(BINDER_NAME, O_RDWR);
    if (fd < 0) {
        LOG("Can't open /dev/binder\n");
    }
    getService();
}

Proxy::~Proxy()
{
    close(fd);
}

void Proxy::getService()
{
    struct binder_get_service_arg arg;

    strncpy(arg.name, name.c_str(), BINDER_SERVICE_NAME_MAX_LEN);
    id = ioctl(fd, BINDER_GET_SERVICE, &arg);
    if (id < 0) {
        LOG("Can't get service");
    }
    else {
        LOG("Get service %s id: %d\n", name.c_str(), id);
    }
}

int Proxy::sendMessage(unsigned char *buf, int size)
{
    struct binder_transaction_data w;
    struct binder_transaction_data r;
    struct binder_write_read wr = {
        .write_buffer = (unsigned long)&w,
        .read_buffer = (unsigned long)&r,
    };

    char reply[256];

    w.cmd = BINDER_TRANSACTION;
    w.id = id;
    w.code = 15;
    w.buffer = buf;
    w.size = size;

    r.cmd = BINDER_REPLY;
    r.id = id;
    r.code = 0;
    r.buffer = reply;
    r.size = sizeof(reply);

    int ret = ioctl(fd, BINDER_WRITE_READ, &wr);
    if (ret < 0) {
        LOG("Can't send message");
    }
    else {
        LOG("Send message %s\n", buf);
    }

    LOG("Client read reply success\n");
    LOG("cmd: %d\n", r.cmd);
    LOG("id: %d\n", r.id);
    LOG("code: %d\n", r.code);
    LOG("buffer char: %s\n", (char *)reply);

    return ret;
}

int main(int argc, char* argv[])
{
    auto proxy = new Proxy("testService");
    char data[] = "Hello, World!";

    proxy->sendMessage((unsigned char *)data, strlen(data));

    return 0;
}
