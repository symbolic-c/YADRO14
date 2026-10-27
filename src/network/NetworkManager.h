#pragma once
#include <core/PlayerManager.h>
#include <network/Packet.h>

class NetworkManager {
public:
    NetworkManager(PlayerManager* pm) : playerManager(pm) {}
    void handleClient(std::shared_ptr<ClientSession> session);

private:
    PlayerManager* playerManager;
    void spawnPlayer(std::shared_ptr<ClientSession> session);
};
