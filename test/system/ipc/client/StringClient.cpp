#include "system/ipc/Binder.h"
#include "system/ipc/Client.h"
#include <iostream>
#include <string>
#include "stdio.h"
#include "string.h"

using namespace wraith::ipc;

class StringClient: public Client {
public:
    StringClient(const std::string &name): Client(name) {
        std::cout << "StringClient::StringClient" << std::endl;
    }
};

int main(int argc, char **argv) {
    std::string service = "string";
    StringClient client(service);
    int maxSize = 1024;

    auto request = std::make_shared<Request>(maxSize);

    request->code = 1;
    request->size = strlen(argv[1]) + 1;
    strcpy((char *)request->buffer, argv[1]);

    auto reply = client.putRequest(request, maxSize);

    std::cout << reply->code << std::endl;
    std::cout << (char *)reply->buffer << std::endl;
    std::cout << reply->size << std::endl;

    return 0;
}