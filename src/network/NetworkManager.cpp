#include <network/NetworkManager.h>
#include <cstdint>
#include <iostream>
#include <vector>
#include <iomanip>
#include <network/AllPackets.h>

#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <sys/socket.h>
    #include <unistd.h>
#endif

void NetworkManager::handleClient(int client_sock) {
    ClientSession session(client_sock);
    std::vector<uint8_t> rawBuffer(8192);

    while (true) {
        int bytesRead = recv(client_sock, (char*)rawBuffer.data(), rawBuffer.size(), 0);

        if (bytesRead <= 0) {
            break; // ошибка
        }

        ByteBuffer buffer(std::vector<uint8_t>(rawBuffer.begin(), rawBuffer.begin() + bytesRead));

        while (buffer.readOffset < buffer.data.size()) {
            uint8_t packetId = buffer.readByte();

            auto packet = PacketFactory::createPacket(packetId);
            if (packet) {
                packet->read(buffer);
                packet->handle(session);

                if (session.isLoggedIn && !session.hasSpawned) {
                    std::cout << "[Сервер] Игрок залогинен. Спавним..." << std::endl;

                    // 1. Отправляем PositionLook (ID 13)
                    Packet13PlayerLookMove pos;
                    pos.x = 8.5;
                    pos.y = 64.0;
                    pos.stance = 65.62;
                    pos.z = 8.5;
                    pos.yaw = 0;
                    pos.pitch = 0;
                    pos.onGround = false;
                    session.sendPacket(pos);

                    // 2. Отправляем чанки (ID 50 + 51)
                    // (Ваш код отправки чанков)

                    session.hasSpawned = true; // Защита от повторной отправки
                }
            } else {
                std::cerr << "Неизвестный пакет: " << (int)packetId << std::endl;
                break;
            }
        }
    }
}