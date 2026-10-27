#include "PlayerManager.h"

#include <algorithm>
#include <network/packets/Packet34EntityTeleport.h>

void PlayerManager::addSession(std::shared_ptr<ClientSession> session) {
    std::lock_guard<std::mutex> lock(mutex);
    sessions.push_back(session);
}

void PlayerManager::removeSession(int32_t entityId) {
    std::lock_guard<std::mutex> lock(mutex);
    sessions.erase(std::remove_if(sessions.begin(), sessions.end(),
        [entityId](const std::shared_ptr<ClientSession>& s) { return s->entityId == entityId; }),
        sessions.end());
}

void PlayerManager::broadcast(Packet& packet, int32_t excludeId) {
    std::lock_guard<std::mutex> lock(mutex);
    for (auto& s : sessions) {
        if (s->entityId != excludeId && s->isLoggedIn) {
            s->sendPacket(packet);
        }
    }
}

std::vector<std::shared_ptr<ClientSession>> PlayerManager::getSessions() {
    std::lock_guard<std::mutex> lock(mutex);
    return sessions; // Возвращаем копию
}

void PlayerManager::broadcastMovement(std::shared_ptr<ClientSession> player) {
    Packet34EntityTeleport teleport;
    teleport.entityId = player->entityId;
    // Координаты в Minecraft передаются как целые числа * 32
    teleport.x = static_cast<int32_t>(player->x * 32.0);
    teleport.y = static_cast<int32_t>(player->y * 32.0);
    teleport.z = static_cast<int32_t>(player->z * 32.0);
    // Углы переводятся из градусов в 0-255
    teleport.yaw = static_cast<int8_t>(player->yaw / 360.0f * 256.0f);
    teleport.pitch = static_cast<int8_t>(player->pitch / 360.0f * 256.0f);

    broadcast(teleport, player->entityId); // Всем, кроме себя
}