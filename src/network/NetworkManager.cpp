#include <network/NetworkManager.h>
#include <cstdint>
#include <iostream>
#include <vector>
#include <iomanip>
#include <thread>
#include <chrono>
#include <random>
#include <network/AllPackets.h>

#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <sys/socket.h>
    #include <unistd.h>
#endif

// --- ФУНКЦИИ ГЕНЕРАЦИИ (должны быть ДО handleClient) ---

std::vector<uint8_t> generateFlatChunk() {
    std::vector<uint8_t> data;
    data.reserve(82176);
    for (int x = 0; x < 16; ++x) {
        for (int z = 0; z < 16; ++z) {
            for (int y = 0; y < 128; ++y) {
                if (y == 0) data.push_back(0x07);
                else if (y <= 5) data.push_back(0x01);
                else data.push_back(0x00);
            }
        }
    }
    for (int i = 0; i < 16384; ++i) data.push_back(0x00);
    for (int i = 0; i < 16384; ++i) data.push_back(0x00);
    for (int i = 0; i < 16384; ++i) data.push_back(0xFF);
    for (int i = 0; i < 256; ++i) data.push_back(0x01);
    return data;
}

std::vector<uint8_t> generateRandomSafeChunkData() {
    std::vector<uint8_t> data;
    data.reserve(82176);

    const std::vector<uint8_t> safeBlocks = {
        1, 2, 3, 4, 5, 7, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 24, 35,
        41, 42, 43, 44, 45, 47, 48, 49, 56, 57, 58, 80, 82, 87, 88, 89
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> blockDist(0, safeBlocks.size() - 1);

    for (int x = 0; x < 16; ++x) {
        for (int z = 0; z < 16; ++z) {
            for (int y = 0; y < 128; ++y) {
                if (y == 0) data.push_back(7);
                else data.push_back(safeBlocks[blockDist(gen)]);
            }
        }
    }
    for (int i = 0; i < 16384; ++i) data.push_back(0x00);
    for (int i = 0; i < 16384; ++i) data.push_back(0x00);
    for (int i = 0; i < 16384; ++i) data.push_back(0xFF);
    for (int i = 0; i < 256; ++i) data.push_back(0x01);
    return data;
}

// --- ОСНОВНОЙ ЦИКЛ ---

void NetworkManager::handleClient(int client_sock) {
    ClientSession session(client_sock);
    std::vector<uint8_t> rawBuffer(8192);
    std::thread updater;
    bool threadStarted = false;

    while (true) {
        int bytesRead = recv(client_sock, (char*)rawBuffer.data(), rawBuffer.size(), 0);

        if (bytesRead <= 0) {
            break;
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

                    // 1. PositionLook
                    Packet13PlayerLookMove pos;
                    pos.x = 30.5;    // Новый X
                    pos.y = 10.0;     // Новый Y
                    pos.stance = 11.62; // Y + 1.62
                    pos.z = 30.5;    // Новый Z
                    pos.yaw = 0; pos.pitch = 0; pos.onGround = false;
                    session.sendPacket(pos);

                    // 2. Чанки 3x3
                    for (int cx = -1; cx <= 1; ++cx) {
                        for (int cz = -1; cz <= 1; ++cz) {
                            Packet50PreChunk pre;
                            pre.x = cx; pre.z = cz; pre.load = true;
                            session.sendPacket(pre);

                            Packet51MapChunk chunk;
                            chunk.x = cx * 16; chunk.z = cz * 16; chunk.y = 0;
                            chunk.width = 15; chunk.height = 127; chunk.length = 15;
                            chunk.rawData = generateFlatChunk();
                            session.sendPacket(chunk);
                        }
                    }

                    session.hasSpawned = true;

                    // 3. ПОТОК ОБНОВЛЕНИЯ (запускаем ТОЛЬКО ЗДЕСЬ)
                    updater = std::thread([&session]() {
                        while (session.isLoggedIn) {
                            std::this_thread::sleep_for(std::chrono::milliseconds(100));
                            if (!session.isLoggedIn) break;

                            std::cout << "[ПОТОК] Обновление чанка 0,0..." << std::endl;

                            Packet50PreChunk pre;
                            pre.x = 0; pre.z = 0; pre.load = true;
                            session.sendPacket(pre);

                            Packet51MapChunk chunk;
                            chunk.x = 0; chunk.z = 0; chunk.y = 0;
                            chunk.width = 15; chunk.height = 127; chunk.length = 15;
                            chunk.rawData = generateRandomSafeChunkData();
                            session.sendPacket(chunk);
                        }
                    });
                    threadStarted = true;
                }
            } else {
                std::cerr << "Неизвестный пакет: " << (int)packetId << std::endl;
                break;
            }
        }
    }

    // Остановка потока при выходе игрока
    session.isLoggedIn = false;
    if (threadStarted && updater.joinable()) {
        updater.join();
    }
}