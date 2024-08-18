#pragma once

//单例实现，一个进程只有一个binder实例

#include <memory>
#include <string>

#include <mutex>
#include "modules/binder.h"

namespace wraith {
namespace ipc {

class Request {
public:
    Request(int dataSize);
    ~Request();
    int code;
    void *buffer;
    int size;
    int maxDataSize;
private:
};

class Reply {
public:
    Reply(int dataSize);
    ~Reply();
    int code;
    void *buffer;
    int size;
};

class Binder {
public:
    static Binder* self();
    void addService(const std::string &name);
    int getService(const std::string &name);
    void write(int cmd, int id, int code, void *buffer, int size);
    void read(int cmd, int id, int *code, void *buffer, int maxSize, int *outSize);
protected:

private:
    explicit Binder();
    ~Binder();
    int fd;
};

}
}