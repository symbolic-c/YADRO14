#pragma once
#include <network/Packet.h>
#include <atomic>
#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <sys/socket.h>
    #include <unistd.h>
#endif


class ClientSession {
public:
    ClientSession(int socket) : client_sock(socket) {}

    void sendPacket(Packet& packet) {
        ByteBuffer buffer;
        buffer.writeByte(static_cast<uint8_t>(packet.getId()));
        packet.write(buffer);
        send(client_sock, (const char*)buffer.data.data(), buffer.data.size(), 0);
        std::cout << "Packet " << static_cast<int>(packet.getId()) << " send" << std::endl;
    }

    std::atomic<bool> isLoggedIn{false};
    bool hasSpawned = false;
    int32_t entityId = 1;
    std::string username;

    double x = 8.5;
    double y = 64.0;
    double z = 8.5;
    float yaw = 0.0f;
    float pitch = 0.0f;
    bool onGround = false;
private:
    int client_sock;
};
