#pragma once
#include <cstdint>

class Server {
public:
    Server(uint16_t port);
    ~Server();
    void start();
private:
    uint16_t port;
};