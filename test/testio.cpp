#include <iostream>

#include <thread>
#include "io.h"

int main(int argc, const char* argv[])
{
    std::cout << "Hello, world!" << std::endl;

    IO clientA;
    IO clientB;

    clientA.connect(&clientB);
    clientB.connect(&clientA);

    std::thread t1([&clientB](){
        char buffer[2048];
        int len = clientB.read(buffer);
        std::cout << "clientB read: " << len << ", " << buffer << std::endl;
        strcpy(buffer, "Here is a reply.");
        clientB.write(buffer, len);
    });

    char data[2048] = "Here is a message.";
    clientA.write(data, strlen(data));
    int len = clientA.read(data);
    std::cout << "clientA read: " << len << ", " << data << std::endl;

    t1.join();

    return 0;
}