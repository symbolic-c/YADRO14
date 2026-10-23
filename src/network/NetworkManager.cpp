#include "NetworkManager.h"

#include <cstdint>
#include <iostream>
#include <vector>
#include <iomanip>

#include "AllPackets.h"

#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <sys/socket.h>
    #include <unistd.h>
#endif

class ClientSession {};

void NetworkManager::handleClient(int client_sock) {
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

                ClientSession dummySession;
                packet->handle(dummySession);
            } else {
                std::cerr << "Неизвестный пакет: " << (int)packetId << std::endl;
                break;
            }
        }
    }
}
