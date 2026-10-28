#pragma once
#include <vector>
#include <mutex>
#include <memory>

#include "ClientSession.h"

class PlayerManager {
public:
    void addSession(std::shared_ptr<ClientSession> session);
    void removeSession(int32_t entityId);

    // Разослать пакет всем, кроме указанного ID
    void broadcast(Packet& packet, int32_t excludeId = -1);

    // Получить копию списка для безопасного перебора
    std::vector<std::shared_ptr<ClientSession>> getSessions();

    // Трансляция движения конкретного игрока
    void broadcastMovement(ClientSession& session);
    void broadcastRotation(ClientSession& session);

private:
    std::vector<std::shared_ptr<ClientSession>> sessions;
    std::mutex mutex;
};
