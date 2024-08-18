#include "system/ipc/Service.h"
#include <cstdlib>
#include <exception>
#include <iostream>     // 所有的iostream都要改成自研的日志系统
#include <memory>
#include <type_traits>

namespace wraith {
namespace ipc {

void Service::init() {
    Binder::self()->addService(name);
    maxDataSize = 1024;
    onCreate();
}

Service::Service(const std::string &name):
    name(name) {
    init();
}

Service::~Service() {
}

// 开始服务线程，目前只能单线程, TODO: 支持多线程
// 目前直接占用了调用start的线程，没有打开新的线程
void Service::start() {
    onStart();
#if 1
    workLoop();
#else
    thread = std::make_shared<std::thread>([this]() {
        workLoop();
    });
#endif
}

// 服务进程创建时调用
void Service::onCreate() {

}

// 每一条服务的工作线程开始时调用
void Service::onStart() {

}

// 每一条服务的工作线程结束时调用
void Service::onStop() {

}

// 服务进程退出时调用
void Service::onDestroy() {

}

std::shared_ptr<Reply> Service::onRequest(std::shared_ptr<Request> request) {
    return nullptr;
}

std::shared_ptr<Request> Service::getRequest() {
    auto request = std::make_shared<Request>(maxDataSize);
    Binder::self()->read(BINDER_TRANSACTION, 0, &request->code, request->buffer, maxDataSize, &request->size);
    return request;
}

void Service::replyRequest(std::shared_ptr<Reply> reply) {
    Binder::self()->write(BINDER_REPLY, 0, reply->code, reply->buffer, reply->size);
}

void Service::workLoop() {
    while (true) {
        try {
            // 这里一旦进入等待请求的休眠，就没办法线程退出了
            auto request = getRequest();
            auto reply = onRequest(request);
            replyRequest(reply);
        } catch (std::exception &e) {
            std::cout << "Service::workLoop() exception: " << e.what() << std::endl;
        }
    }
}

}
}