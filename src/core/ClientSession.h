#pragma once
#include <network/Packet.h>
#include <atomic>
#include <iostream>
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
        std::lock_guard<std::mutex> lock(sendMutex);


        ByteBuffer buffer;
        buffer.writeByte(static_cast<uint8_t>(packet.getId()));
        packet.write(buffer);
        int sent = send(client_sock, (const char*)buffer.data.data(), buffer.data.size(), 0);
        if (sent < 0) {
            std::cerr << "[Сокет] Ошибка отправки пакета " << (int)packet.getId() << std::endl;
        }
    }

    // --- Состояние игрока ---
    std::atomic<bool> isLoggedIn{false};
    std::atomic<bool> hasSpawned{false};
    int32_t entityId = 0;
    std::string username;

    // Координаты и поворот
    double x = 8.5;
    double y = 4.0; // Спавн на траве
    double z = 8.5;
    float yaw = 0.0f;
    float pitch = 0.0f;
    bool onGround = false;

    int getSocket() const { return client_sock; }
private:
    int client_sock;
    std::mutex sendMutex;
};
