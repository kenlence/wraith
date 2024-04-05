/* Binder test cases
 *
 */


#include <iostream>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "binder.h"
#include <unistd.h>
#include "string.h"

#define BINDER_NAME "/dev/binder"
#define SERVICE_NAME "binder_tester"

class BinderTester {
public:
    BinderTester() {
        fd = open(BINDER_NAME, O_RDWR );
        if (fd < 0) {
            std::cout << "Can't open /dev/binder" << std::endl;
        }
    };

    void addService() {
        struct binder_add_service_arg arg;

        strncpy(arg.name, SERVICE_NAME, BINDER_SERVICE_NAME_MAX_LEN);
        ioctl(fd, BINDER_ADD_SERVICE, &arg);
    }

private:
    int fd;
};

int main()
{
    BinderTester test;
    test.addService();

    while(1)
    {
        sleep(10);
    }

    return 0;
}