#include "Server.h"

#include <iostream>
#include <network/packets/Packet29DestroyEntity.h>
#include <network/packets/Packet3Chat.h>

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

