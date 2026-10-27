#pragma once
#include <cstdint>
#include <string>
#include <core/ClientSession.h>


class Player {
public:
    int32_t entityId;
    std::string username;

    // Позиция
    double x = 8.5;
    double y = 4.0;
    double z = 8.5;
    float yaw = 0.0f;
    float pitch = 0.0f;
    double lastBroadcastX = 8.5;
    double lastBroadcastY = 4.0;
    double lastBroadcastZ = 8.5;
    // Состояние
    bool onGround = false;
    bool isSneaking = false;
    bool isSprinting = false;

    // Здоровье
    float health = 20.0f;

    Player(int32_t id, const std::string& name) : entityId(id), username(name) {}
};
