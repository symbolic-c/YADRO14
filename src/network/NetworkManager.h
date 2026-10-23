#pragma once
#include <network/Packet.h>

class NetworkManager {
public:
    void handleClient(int client_sock);
    void sendPacket(int client_sock, Packet& packet);
};
