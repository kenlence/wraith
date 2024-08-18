#pragma once

#include <memory>
#include <string>
#include "Binder.h"
#include <thread>

namespace wraith {
namespace ipc {

class Service {
public:
    Service(const std::string &name);
    ~Service();
    void start();

protected:
    virtual void onCreate();
    virtual void onStart();
    virtual std::shared_ptr<Reply> onRequest(std::shared_ptr<Request> request);
    virtual void onStop();
    virtual void onDestroy();

private:
    std::string name;
    int maxDataSize;
    std::shared_ptr<std::thread> thread;
    void init();
    void workLoop();
    std::shared_ptr<Request> getRequest();
    void replyRequest(std::shared_ptr<Reply> reply);
};

}
}