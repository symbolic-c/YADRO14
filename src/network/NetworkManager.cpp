#include <network/NetworkManager.h>
#include <cstdint>
#include <iostream>
#include <vector>
#include <iomanip>
#include <thread>
#include <chrono>
#include <random>
#include <network/AllPackets.h>
#include <world/FlatWorldGenerator.h>

#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <sys/socket.h>
    #include <unistd.h>
#endif

// --- ФУНКЦИИ ГЕНЕРАЦИИ (должны быть ДО handleClient) ---
//
// std::vector<uint8_t> generateFlatChunk() {
//     std::vector<uint8_t> data;
//     data.reserve(82176);
//     for (int x = 0; x < 16; ++x) {
//         for (int z = 0; z < 16; ++z) {
//             for (int y = 0; y < 128; ++y) {
//                 if (y == 0) data.push_back(0x07);
//                 else if (y <= 5) data.push_back(0x01);
//                 else data.push_back(0x00);
//             }
//         }
//     }
//     for (int i = 0; i < 16384; ++i) data.push_back(0x00);
//     for (int i = 0; i < 16384; ++i) data.push_back(0x00);
//     for (int i = 0; i < 16384; ++i) data.push_back(0xFF);
//     for (int i = 0; i < 256; ++i) data.push_back(0x01);
//     return data;
// }
//
// std::vector<uint8_t> generateRandomSafeChunkData() {
//     std::vector<uint8_t> data;
//     data.reserve(82176);
//
//     const std::vector<uint8_t> safeBlocks = {
//         1, 2, 3, 4, 5, 7, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 24, 35,
//         41, 42, 43, 44, 45, 47, 48, 49, 56, 57, 58, 80, 82, 87, 88, 89
//     };
//
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::uniform_int_distribution<> blockDist(0, safeBlocks.size() - 1);
//
//     for (int x = 0; x < 16; ++x) {
//         for (int z = 0; z < 16; ++z) {
//             for (int y = 0; y < 128; ++y) {
//                 if (y == 0) data.push_back(7);
//                 else data.push_back(safeBlocks[blockDist(gen)]);
//             }
//         }
//     }
//     for (int i = 0; i < 16384; ++i) data.push_back(0x00);
//     for (int i = 0; i < 16384; ++i) data.push_back(0x00);
//     for (int i = 0; i < 16384; ++i) data.push_back(0xFF);
//     for (int i = 0; i < 256; ++i) data.push_back(0x01);
//     return data;
// }

// --- ОСНОВНОЙ ЦИКЛ ---

// void NetworkManager::handleClient(int client_sock) {
//     ClientSession session(client_sock);
//     std::vector<uint8_t> rawBuffer(8192);
//     std::thread updater;
//     bool threadStarted = false;
//
//     while (true) {
//         int bytesRead = recv(client_sock, (char*)rawBuffer.data(), rawBuffer.size(), 0);
//
//         if (bytesRead <= 0) {
//             break;
//         }
//
//         ByteBuffer buffer(std::vector<uint8_t>(rawBuffer.begin(), rawBuffer.begin() + bytesRead));
//
//         while (buffer.readOffset < buffer.data.size()) {
//             uint8_t packetId = buffer.readByte();
//
//             auto packet = PacketFactory::createPacket(packetId);
//             if (packet) {
//                 packet->read(buffer);
//                 packet->handle(session);
//
//                 if (session.isLoggedIn && !session.hasSpawned) {
//                     std::cout << "[Сервер] Игрок залогинен. Спавним..." << std::endl;
//
//                     // 1. PositionLook
//                     Packet13PlayerLookMove pos;
//                     pos.x = 30.5;    // Новый X
//                     pos.y = 10.0;     // Новый Y
//                     pos.stance = 11.62; // Y + 1.62
//                     pos.z = 30.5;    // Новый Z
//                     pos.yaw = 0; pos.pitch = 0; pos.onGround = false;
//                     session.sendPacket(pos);
//
//                     // 2. Чанки 3x3
//                     for (int cx = -1; cx <= 1; ++cx) {
//                         for (int cz = -1; cz <= 1; ++cz) {
//                             Packet50PreChunk pre;
//                             pre.x = cx; pre.z = cz; pre.load = true;
//                             session.sendPacket(pre);
//
//                             Packet51MapChunk chunk;
//                             chunk.x = cx * 16; chunk.z = cz * 16; chunk.y = 0;
//                             chunk.width = 15; chunk.height = 127; chunk.length = 15;
//                             chunk.rawData = generateFlatChunk();
//                             session.sendPacket(chunk);
//                         }
//                     }
//
//                     session.hasSpawned = true;
//
//                     // 3. ПОТОК ОБНОВЛЕНИЯ (запускаем ТОЛЬКО ЗДЕСЬ)
//                     updater = std::thread([&session]() {
//                         while (session.isLoggedIn) {
//                             std::this_thread::sleep_for(std::chrono::milliseconds(100));
//                             if (!session.isLoggedIn) break;
//
//                             std::cout << "[ПОТОК] Обновление чанка 0,0..." << std::endl;
//
//                             Packet50PreChunk pre;
//                             pre.x = 0; pre.z = 0; pre.load = true;
//                             session.sendPacket(pre);
//
//                             Packet51MapChunk chunk;
//                             chunk.x = 0; chunk.z = 0; chunk.y = 0;
//                             chunk.width = 15; chunk.height = 127; chunk.length = 15;
//                             chunk.rawData = generateRandomSafeChunkData();
//                             session.sendPacket(chunk);
//                         }
//                     });
//                     threadStarted = true;
//                 }
//             } else {
//                 std::cerr << "Неизвестный пакет: " << (int)packetId << std::endl;
//                 break;
//             }
//         }
//     }
//
//     // Остановка потока при выходе игрока
//     session.isLoggedIn = false;
//     if (threadStarted && updater.joinable()) {
//         updater.join();
//     }
// }
std::vector<uint8_t> World::generateChunkData(int chunkX, int chunkZ) {
    std::vector<uint8_t> data;
    data.reserve(82176);

    // Блокируем мьютекс ОДИН раз на весь чанк (для производительности)
    std::lock_guard<std::mutex> lock(worldMutex);

    for (int x = 0; x < 16; ++x) {
        for (int z = 0; z < 16; ++z) {
            for (int y = 0; y < 128; ++y) {
                // Глобальные координаты блока в мире
                int worldX = chunkX * 16 + x;
                int worldZ = chunkZ * 16 + z;

                auto key = std::make_tuple(worldX, y, worldZ);
                uint8_t blockId = 0x00; // По умолчанию воздух

                // 1. Проверяем, изменял ли игрок этот блок
                if (blocks.find(key) != blocks.end()) {
                    blockId = blocks[key];
                } else {
                    // 2. Если нет, генерируем базовый ландшафт (плоский мир)
                    if (y == 0) blockId = 0x07;      // Бедрок
                    else if (y == 1 || y == 2) blockId = 0x03; // Земля
                    else if (y == 3) blockId = 0x02; // Трава
                    else blockId = 0x00;             // Воздух
                }

                data.push_back(blockId);
            }
        }
    }

    // Метаданные (16384)
    for (int i = 0; i < 16384; ++i) data.push_back(0x00);
    // Свет блоков (16384)
    for (int i = 0; i < 16384; ++i) data.push_back(0x00);
    // Небесный свет (16384)
    for (int i = 0; i < 16384; ++i) data.push_back(0xFF);
    // Биомы (256)
    for (int i = 0; i < 256; ++i) data.push_back(0x01);

    return data;
}

void sendSingleChunk(ClientSession& session, int cx, int cz) {
    Packet50PreChunk pre;
    pre.x = cx;
    pre.z = cz;
    pre.load = true;
    session.sendPacket(pre);

    Packet51MapChunk chunk;
    chunk.x = cx * 16;
    chunk.z = cz * 16;
    chunk.y = 0;
    chunk.width = 15;
    chunk.height = 127;
    chunk.length = 15;

    // БЕРЕМ ДАННЫЕ ИЗ МИРА, А НЕ ИЗ ГЕНЕРАТОРА!
    chunk.rawData = session.world->generateChunkData(cx, cz);

    session.sendPacket(chunk);
}


void NetworkManager::spawnPlayer(std::shared_ptr<ClientSession> session) {
    if (!session->player) {
        std::cerr << "[Ошибка] Попытка спавна игрока без объекта Player!" << std::endl;
        return;
    }

    std::cout << "[Сервер] Спавн игрока " << session->player->username << std::endl;

    // 1. Отправляем PositionLook (ID 13)
    Packet13PlayerLookMove pos;
    pos.x = session->player->x;
    pos.y = session->player->y;
    pos.stance = session->player->y + 1.62;
    pos.z = session->player->z;
    pos.yaw = 0;
    pos.pitch = 0;
    pos.onGround = false;
    session->sendPacket(pos);

    int chunkCoords[2] = {0, 1};

    for (int cx : chunkCoords) {
        for (int cz : chunkCoords) {
            // PreChunk (ID 50)
            Packet50PreChunk pre;
            pre.x = cx;
            pre.z = cz;
            pre.load = true;
            session->sendPacket(pre);

            // MapChunk (ID 51)
            Packet51MapChunk chunk;
            chunk.x = cx * 16;
            chunk.z = cz * 16;
            chunk.y = 0;
            chunk.width = 15;
            chunk.height = 127;
            chunk.length = 15;
            // БЕРЕМ ДАННЫЕ ИЗ МИРА
            chunk.rawData = session->world->generateChunkData(cx, cz);
            session->sendPacket(chunk);
        }
    }

    // 3. Показываем новому игроку всех, кто уже в игре
    for (auto& other : playerManager->getSessions()) {
        if (other->player && other->player->entityId != session->player->entityId && other->hasSpawned) {
            Packet20NamedEntitySpawn spawnOther;
            spawnOther.entityId = other->player->entityId;
            spawnOther.name = other->player->username;
            spawnOther.x = static_cast<int32_t>(other->player->x * 32.0);
            spawnOther.y = static_cast<int32_t>(other->player->y * 32.0);
            spawnOther.z = static_cast<int32_t>(other->player->z * 32.0);
            spawnOther.rotation = static_cast<int8_t>(other->player->yaw / 360.0f * 256.0f);
            spawnOther.pitch = static_cast<int8_t>(other->player->pitch / 360.0f * 256.0f);
            spawnOther.heldItem = 0;
            session->sendPacket(spawnOther);
        }
    }

    // 4. Показываем нового игрока всем остальным
    Packet20NamedEntitySpawn mySpawn;
    mySpawn.entityId = session->player->entityId;
    mySpawn.name = session->player->username;
    mySpawn.x = static_cast<int32_t>(session->player->x * 32.0);
    mySpawn.y = static_cast<int32_t>(session->player->y * 32.0);
    mySpawn.z = static_cast<int32_t>(session->player->z * 32.0);
    mySpawn.rotation = static_cast<int8_t>(session->player->yaw / 360.0f * 256.0f);
    mySpawn.pitch = static_cast<int8_t>(session->player->pitch / 360.0f * 256.0f);
    mySpawn.heldItem = 0;
    playerManager->broadcast(mySpawn, session->player->entityId);
}

void NetworkManager::handleClient(std::shared_ptr<ClientSession> session) {
    std::vector<uint8_t> rawBuffer(8192);

    // Основной цикл чтения
    while (true) {
        int bytesRead = recv(session->getSocket(), (char*)rawBuffer.data(), rawBuffer.size(), 0);
        if (bytesRead <= 0) break;

        ByteBuffer buffer(std::vector<uint8_t>(rawBuffer.begin(), rawBuffer.begin() + bytesRead));

        while (buffer.readOffset < buffer.data.size()) {
            uint8_t packetId = buffer.readByte();
            auto packet = PacketFactory::createPacket(packetId);

            if (packet) {
                packet->read(buffer);

                packet->handle(*session->player, *session);


                // Если игрок только что залогинился, спавним его
                if (session->isLoggedIn && !session->hasSpawned) {
                    session->hasSpawned = true;
                    spawnPlayer(session);
                }


            } else {
                std::cerr << "Неизвестный пакет: " << (int)packetId << std::endl;
                break;
            }
        }
    }
}
