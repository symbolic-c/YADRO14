#include "NetworkManager.h"

#include <cstdint>
#include <iostream>
#include <vector>
#include <iomanip>

#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <sys/socket.h>
    #include <unistd.h>
#endif

void NetworkManager::handleClient(int client_sock) {
    std::vector<uint8_t> buffer(8192);

    while (true) {
        int bytesRead = recv(client_sock, (char*)buffer.data(), buffer.size(), 0);

        if (bytesRead <= 0) {
            break; // ошибка
        }

        std::cout << "Получено " << bytesRead << " байт: ";

        // вывод байтов
        for (int i = 0; i < bytesRead; ++i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0')
                      << (int)buffer[i] << " ";
        }
        std::cout << std::dec << std::endl;

        // ответ на KeepAlive
        if (buffer[0] == 0x00) {
            uint8_t keepAlive = 0x00;
            send(client_sock, (const char*)&keepAlive, 1, 0);
        }
    }
}
