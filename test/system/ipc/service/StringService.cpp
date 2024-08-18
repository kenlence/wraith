#include "system/ipc/Service.h"
#include <iostream>
#include <string>
#include "stdio.h"
#include "string.h"

using namespace wraith::ipc;

class StringService: public Service {
public:
    StringService(const std::string &name): Service(name) {
        std::cout << "StringService::StringService" << std::endl;
    }

    void onCreate() {
        std::cout << "StringService::onCreate" << std::endl;
    }

    void onStart() {
        std::cout << "StringService::onStart" << std::endl;
    }

    std::shared_ptr<Reply> onRequest(std::shared_ptr<Request> request) {
        std::shared_ptr<Reply> reply = std::make_shared<Reply>(1024);
        std::cout << "StringService::onRequest: " << std::endl;
        std::cout << request->code << std::endl;
        std::cout << (char *)request->buffer << std::endl;
        std::cout << request->size << std::endl;
        std::cout << request->maxDataSize << std::endl;
        snprintf((char *)reply->buffer, 1024, "Hello, %s", (char *)request->buffer);
        reply->size = strlen((char *)reply->buffer) + 1;
        reply->code = request->code;
        return reply;
    }
};

int main() {
    std::string name = "string";
    StringService service(name);
    std::cout << "StringService::start " << std::endl;
    service.start();
    return 0;
}