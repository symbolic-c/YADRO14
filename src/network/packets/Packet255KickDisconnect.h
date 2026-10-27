#pragma once
#include <iostream>
#include <network/Packet.h>

#include "Packet29DestroyEntity.h"

class Packet255KickDisconnect : public Packet{
public:
    std::string reason;
    PacketId getId() const override { return PacketId::KickDisconnect; }

    void read(ByteBuffer &buffer) override {
        reason = buffer.readString();
    }

    void write(ByteBuffer &buffer) const override {
        buffer.writeString(reason);
    }

    void handle(Player& player, ClientSession &session) override {
        std::cout << "[Сервер] " << player.username << " вышел из игры." << std::endl;

        // 1. Рассылаем сообщение в чат немедленно
        Packet3Chat leaveMsg;
        leaveMsg.message = player.username + " left the game.";
        session.playerManager->broadcast(leaveMsg, -1);

        // 2. Удаляем модель игрока у всех
        Packet29DestroyEntity destroy;
        destroy.entityId = player.entityId;
        session.playerManager->broadcast(destroy, -1);

        // 3. Помечаем сессию как "мертвую", чтобы Server.cpp не рассылал сообщение повторно
        session.hasQuit = true; // <--- Нужно добавить это поле в ClientSession
    }
};
