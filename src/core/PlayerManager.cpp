#include "PlayerManager.h"

#include <algorithm>
#include <cmath>
#include <network/packets/Packet31RelEntityMove.h>
#include <network/packets/Packet34EntityTeleport.h>

void PlayerManager::addSession(std::shared_ptr<ClientSession> session) {
    std::lock_guard<std::mutex> lock(mutex);
    sessions.push_back(session);
}

void PlayerManager::removeSession(int32_t entityId) {
    std::lock_guard<std::mutex> lock(mutex);
    sessions.erase(std::remove_if(sessions.begin(), sessions.end(),
        [entityId](const std::shared_ptr<ClientSession>& s) {
            // Проверяем, что player существует, и сравниваем его ID
            return s->player && s->player->entityId == entityId;
        }),
        sessions.end());
}

void PlayerManager::broadcast(Packet& packet, int32_t excludeId) {
    std::lock_guard<std::mutex> lock(mutex);
    for (auto& s : sessions) {
        // Проверяем, что игрок есть, он залогинен, и это не тот, кого мы исключаем
        if (s->player && s->player->entityId != excludeId && s->isLoggedIn) {
            s->sendPacket(packet);
        }
    }
}

std::vector<std::shared_ptr<ClientSession>> PlayerManager::getSessions() {
    std::lock_guard<std::mutex> lock(mutex);
    return sessions; // Возвращаем копию
}

void PlayerManager::broadcastMovement(std::shared_ptr<ClientSession> session) {
    if (!session->player) return;

    Player& p = *session->player;

    // Вычисляем разницу между текущей позицией и последней отправленной
    double deltaX = p.x - p.lastBroadcastX;
    double deltaY = p.y - p.lastBroadcastY;
    double deltaZ = p.z - p.lastBroadcastZ;

    // Переводим в "шаги" по 1/32 блока и округляем
    int32_t stepX = static_cast<int32_t>(std::round(deltaX * 32.0));
    int32_t stepY = static_cast<int32_t>(std::round(deltaY * 32.0));
    int32_t stepZ = static_cast<int32_t>(std::round(deltaZ * 32.0));

    // Если игрок сдвинулся слишком мало или не двигался вовсе - ничего не шлем
    if (stepX == 0 && stepY == 0 && stepZ == 0) return;

    // Если смещение по какой-то оси больше 127 (или меньше -128),
    // Packet31 не сможет это передать (он использует byte).
    // В этом случае шлем Packet34 (абсолютную телепортацию).
    if (stepX > 127 || stepX < -128 || stepY > 127 || stepY < -128 || stepZ > 127 || stepZ < -128) {
        Packet34EntityTeleport teleport;
        teleport.entityId = p.entityId;
        teleport.x = static_cast<int32_t>(p.x * 32.0);
        teleport.y = static_cast<int32_t>(p.y * 32.0);
        teleport.z = static_cast<int32_t>(p.z * 32.0);
        teleport.yaw = static_cast<int8_t>(p.yaw / 360.0f * 256.0f);
        teleport.pitch = static_cast<int8_t>(p.pitch / 360.0f * 256.0f);
        broadcast(teleport, p.entityId);

        // Обновляем "последнюю" позицию
        p.lastBroadcastX = p.x;
        p.lastBroadcastY = p.y;
        p.lastBroadcastZ = p.z;
        return;
    }

    // Если смещение небольшое, шлем плавное движение (Packet31)
    Packet31RelEntityMove move;
    move.entityId = p.entityId;
    move.dx = static_cast<int8_t>(stepX);
    move.dy = static_cast<int8_t>(stepY);
    move.dz = static_cast<int8_t>(stepZ);
    broadcast(move, p.entityId);

    // Обновляем "последнюю" позицию, чтобы следующая дельта считалась от нее
    p.lastBroadcastX = p.x;
    p.lastBroadcastY = p.y;
    p.lastBroadcastZ = p.z;
}