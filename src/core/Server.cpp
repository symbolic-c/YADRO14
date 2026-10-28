#include "Server.h"

#include <iostream>
#include <network/packets/Packet103SetSlot.h>
#include <network/packets/Packet22Collect.h>
#include <network/packets/Packet29DestroyEntity.h>
#include <network/packets/Packet3Chat.h>
#include <network/packets/Packet4UpdateTime.h>

#include "ClientSession.h"
#include "../network/NetworkManager.h"

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

Server::Server(uint16_t port) : port(port) {}

void Server::start() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Ошибка WSAStartup" << std::endl;
        return;
    }
#endif

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0 ) {
        std::cerr << "Ошибка создания сокета" << std::endl;
        return;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "Ошибка bind" << std::endl;
        return;
    }

    listen(server_fd, 5);
    std::cout << "Сервер запущен на порту " << port << ". Ожидание клиента..." << std::endl;
    std::thread tickThread([this]() {
        int64_t worldTime = 6000;

        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));

            worldTime++;
            if (worldTime >= 24000) worldTime = 0;

            // 1. Отправляем время
            Packet4UpdateTime timePacket(worldTime);
            for (auto& session : playerManager.getSessions()) {
                if (session->isLoggedIn) {
                    session->sendPacket(timePacket);
                }
            }

            // 2. Логика подбора предметов (раз в тик)
            auto items = world.getDroppedItems();
            for (auto it = items.begin(); it != items.end(); ) {
                bool pickedUp = false;
                for (auto& session : playerManager.getSessions()) {
                    if (!session->player || !session->isLoggedIn) continue;

                    double dx = session->player->x - it->x;
                    double dy = session->player->y - it->y;
                    double dz = session->player->z - it->z;

                    // Если игрок близко (меньше 1.5 блока)
                    if (dx*dx + dy*dy + dz*dz < 2.25) {
                        // 1. СНАЧАЛА ИЩЕМ СЛОТ В ИНВЕНТАРЕ
                        // 1. Ищем пустой слот в вашем массиве (0-35)
                    int targetSlot = -1;
                    for (int i = 0; i < 36; ++i) {
                        if (session->player->inventory[i].isEmpty()) {
                            targetSlot = i;
                            break;
                        }
                    }

                    if (targetSlot == -1) {
                        std::cout << "[Сервер] Инвентарь полон!" << std::endl;
                        continue;
                    }

                    // 2. Кладем предмет в серверный массив
                    session->player->inventory[targetSlot].id = it->itemId;
                    session->player->inventory[targetSlot].count = it->count;
                    session->player->inventory[targetSlot].damage = 0;

                    // 3. ПЕРЕВОДИМ В СЕТЕВОЙ СЛОТ
                    int networkSlot = targetSlot;
                    if (targetSlot >= 0 && targetSlot <= 8) {
                        // Если это хотбар (0-8), прибавляем 36
                        networkSlot = 36 + targetSlot;
                    }
                    // Если это основной инвентарь (9-35), индекс остается тем же (9-35)

                    // 4. Отправляем пакет с ПРАВИЛЬНЫМ сетевым слотом
                    Packet103SetSlot slotPacket;
                    slotPacket.windowId = 0;
                    slotPacket.slot = networkSlot; // <--- Используем networkSlot
                    slotPacket.item = session->player->inventory[targetSlot];
                    session->sendPacket(slotPacket);

                        // 5. ТОЛЬКО ТЕПЕРЬ ОТПРАВЛЯЕМ АНИМАЦИЮ И УДАЛЯЕМ ПРЕДМЕТ
                        Packet22Collect collect;
                        collect.collectedEntityId = it->entityId;
                        collect.collectorEntityId = session->player->entityId;
                        playerManager.broadcast(collect, -1);

                        Packet29DestroyEntity destroy;
                        destroy.entityId = it->entityId;
                        playerManager.broadcast(destroy, -1);

                        world.removeItem(it->entityId);
                        pickedUp = true;
                        break;
                    }

                    }

                if (pickedUp) {
                it = items.erase(it); // Удаляем из локальной копии
                } else {
                    ++it;
                }
                }
            }
    });
    tickThread.detach();

    while (true) {
        sockaddr_in client_addr{};
#ifdef _WIN32
        int client_len = sizeof(client_addr);
#else
        socklen_t client_len = sizeof(client_addr);
#endif

        int client_sock = accept(server_fd, (sockaddr*)&client_addr, &client_len);
        if (client_sock < 0) continue;

        std::cout << "[Сервер] Новое подключение! Создан поток." << std::endl;

        auto session = std::make_shared<ClientSession>(client_sock);
        session->player = std::make_shared<Player>(nextEntityId++, "Unknown");
        session->playerManager = &playerManager;
        session->world = &world;
        playerManager.addSession(session);

        std::thread([this, session]() {
            NetworkManager network(&playerManager);
            network.handleClient(session);

            int32_t id = session->player ? session->player->entityId : 0;
            std::string name = session->player ? session->player->username : "Unknown";

            playerManager.removeSession(id);

            if (!session->hasQuit) {
                Packet29DestroyEntity destroy;
                destroy.entityId = id;
                playerManager.broadcast(destroy, -1);

                Packet3Chat leaveMsg;
                leaveMsg.message = name + " left the game (connection lost).";
                playerManager.broadcast(leaveMsg, -1);
            }
#ifdef _WIN32
            closesocket(session->getSocket());
#else
            close(session->getSocket());
#endif
            std::cout << "[Сервер] Поток игрока " << id << " завершен." << std::endl;
        }).detach();
    }
#ifdef _WIN32
    WSACleanup();
#endif
}

