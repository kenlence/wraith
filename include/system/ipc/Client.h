#pragma once

#include "Binder.h"

namespace wraith {
namespace ipc {

class Client {
public:
    Client(const std::string &name);  // 这个name是对应服务的name
    ~Client();
    std::shared_ptr<Reply> putRequest(std::shared_ptr<Request> request, int maxReplySize);

protected:
    void onCreate();
    void onDestroy();

private:
    int serviceId;
    int maxDataSize;
};

}
}