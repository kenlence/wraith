#include "system/ipc/Binder.h"
#include <cstdlib>
#include <fcntl.h>
#include <stdexcept>
#include <unistd.h>
#include "string.h"
#include <sys/ioctl.h>
#include "stdio.h"
#include <iostream>

namespace wraith {
namespace ipc {

const char *binder_device = "/dev/binder";
static std::mutex gBinderMutex;
static Binder *gBinder;

Binder* Binder::self() {
    std::lock_guard<std::mutex> lock(gBinderMutex);

    if (gBinder == nullptr) {
        gBinder = new Binder();
    }

    return gBinder;
}

Binder::Binder() {
    fd = open(binder_device, O_RDWR);
    if (fd < 0) {
        throw std::runtime_error("open binder device failed");
    }
}

Binder::~Binder() {
    close(fd);
}

void Binder::addService(const std::string &name) {
    struct binder_add_service_arg arg;

    strncpy(arg.name, name.c_str(), BINDER_SERVICE_NAME_MAX_LEN);
    if (ioctl(fd, BINDER_ADD_SERVICE, &arg) < 0) {
        throw std::runtime_error("Add service ioctl failed");
    }
}

int Binder::getService(const std::string &name) {
    int id;
    struct binder_get_service_arg arg;

    strncpy(arg.name, name.c_str(), BINDER_SERVICE_NAME_MAX_LEN);
    id = ioctl(fd, BINDER_GET_SERVICE, &arg);
    if (id < 0) {
        throw std::runtime_error("Get service ioctl failed");
    }

    return id;
}

void Binder::write(int cmd, int id, int code, void *buffer, int size) {
    struct binder_transaction_data w;
    struct binder_write_read wr = {
        .write_buffer = (unsigned long)&w,
        .read_buffer = 0,
    };

    w.cmd = cmd;
    w.id = id;
    w.code = code;
    w.buffer = buffer;
    w.size = size;

    if (ioctl(fd, BINDER_WRITE_READ, &wr) < 0) {
        throw "Can't write read binder";
    }
}

void Binder::read(int cmd, int id, int *code, void *buffer, int maxSize, int *outSize) {
    struct binder_transaction_data r;
    struct binder_write_read wr = {
        .write_buffer = 0,
        .read_buffer = (unsigned long)&r,
    };

    r.cmd = cmd;
    r.id = id;
    r.code = 0;
    r.buffer = buffer;
    r.size = maxSize;

    if (ioctl(fd, BINDER_WRITE_READ, &wr) < 0) {
        throw "Can't write read binder";
    }


    *code = r.code;
    *outSize = r.size;
}

Request::Request(int dataSize) {
    buffer = malloc(dataSize);
    if (buffer == nullptr) {
        throw std::runtime_error("Can't allocate memory for request");
    }
    maxDataSize = dataSize;
}

Request::~Request()
{
    if (buffer != nullptr)
        free(buffer);
}

Reply::Reply(int dataSize) {
    buffer = malloc(dataSize);
    if (buffer == nullptr) {
        throw std::runtime_error("Can't allocate memory for reply");
    }
    size = dataSize;
}

Reply::~Reply() {
    if (buffer != nullptr)
        free(buffer);
}

}
}