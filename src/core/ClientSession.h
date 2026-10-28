#pragma once
#include <network/Packet.h>
#include <atomic>
#include <iostream>
#include <entity/Player.h>
#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <sys/socket.h>
    #include <unistd.h>
#endif

class World;
class Player;
class PlayerManager;

class ClientSession {
public:
    ClientSession(int socket) : client_sock(socket) {}
    class PlayerManager* playerManager = nullptr;
    World* world = nullptr;
    void sendPacket(Packet& packet) {
        std::lock_guard<std::mutex> lock(sendMutex);


        ByteBuffer buffer;
        buffer.writeByte(static_cast<uint8_t>(packet.getId()));
        packet.write(buffer);
        send(client_sock, (const char*)buffer.data.data(), buffer.data.size(), 0);
    }

    // --- Состояние игрока ---
    std::atomic<bool> isLoggedIn{false};
    std::atomic<bool> hasSpawned{false};
    std::shared_ptr<Player> player;
    std::atomic<bool> hasQuit{false};

    int getSocket() const { return client_sock; }
private:
    int client_sock;
    std::mutex sendMutex;
};
