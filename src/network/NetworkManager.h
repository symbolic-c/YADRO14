#pragma once
#include <network/Packet.h>

class NetworkManager {
public:
    void handleClient(int client_sock);
private:
    void spawnPlayer(ClientSession& session);
    void loadChunksAroundPlayer(ClientSession& session);
};
