#pragma once
#include <cstdint>
#include <string>
#include <core/ClientSession.h>
#include <item/ItemStack.h>


class Player {
public:
    int32_t entityId;
    std::string username;
    ItemStack inventory[36];
    // Позиция
    double x = 8.5;
    double y = 6.0;
    double z = 8.5;
    float yaw = 0.0f;
    float pitch = 0.0f;
    double lastBroadcastX = 8.5;
    double lastBroadcastY = 6.0;
    double lastBroadcastZ = 8.5;
    float lastBroadcastYaw = 0.0f;
    float lastBroadcastPitch = 0.0f;
    // Состояние
    bool onGround = false;
    bool isSneaking = false;
    bool isSprinting = false;

    // Здоровье
    float health = 20.0f;

    Player(int32_t id, const std::string& name) : entityId(id), username(name) {}
};
