#pragma once
#include <cstdint>
#include <thread>

#include "PlayerManager.h"

class Server {
public:
    Server(uint16_t port);
    PlayerManager& getPlayerManager() { return playerManager; }
    void start();
private:
    uint16_t port;
    std::atomic<int32_t> nextEntityId{1};
    PlayerManager playerManager;
};
