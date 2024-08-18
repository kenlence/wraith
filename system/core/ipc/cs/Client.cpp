#include "system/ipc/Client.h"
#include "system/ipc/Binder.h"
#include <iostream>

namespace wraith {
namespace ipc {

Client::Client(const std::string &name) {
    serviceId = Binder::self()->getService(name);
    onCreate();
}

Client::~Client() {
    onDestroy();
}

void Client::onCreate()
{

}

void Client::onDestroy()
{

}

std::shared_ptr<Reply> Client::putRequest(std::shared_ptr<Request> request, int maxReplySize) {
    auto reply = std::make_shared<Reply>(maxReplySize);

    Binder::self()->write(BINDER_TRANSACTION, serviceId, request->code, request->buffer, request->size);
    Binder::self()->read(BINDER_REPLY, serviceId, &reply->code, reply->buffer, maxReplySize, &reply->size);

    if (reply->code != request->code) {
        throw "reply code not match request";
    }
    return reply;
}

}
}