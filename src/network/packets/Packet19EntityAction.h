#pragma once
#include <network/Packet.h>

#include "Packet40EntityMetadata.h"


class Packet19EntityAction : public Packet {
public:
    int32_t entityId = 0;
    int8_t actionId = 0;

    PacketId getId() const override { return PacketId::EntityAction; }

    void read(ByteBuffer &buffer) override {
        entityId = buffer.readInt32();
        actionId = buffer.readByte();
    }

    void write(ByteBuffer &buffer) const override {
        buffer.writeInt32(entityId);
        buffer.writeByte(actionId);
    }

    void handle(Player& player, ClientSession &session) override {
        bool isSneaking = player.isSneaking;

        if (actionId == 1) { // Начал красться
            player.isSneaking = true;
            isSneaking = true;
            std::cout << "[Сервер] " << player.username << " начал красться." << std::endl;
        } else if (actionId == 2) { // Перестал красться
            player.isSneaking = false;
            isSneaking = false;
            std::cout << "[Сервер] " << player.username << " перестал красться." << std::endl;
        } else {
            return; // Другие действия (сон и т.д.) игнорируем
        }

        if (session.playerManager) {
            Packet40EntityMetadata meta;
            meta.entityId = player.entityId;
            meta.setSneaking(isSneaking);

            // Рассылаем всем, КРОМЕ самого игрока (он уже знает, что присел)
            session.playerManager->broadcast(meta, player.entityId);
        }
    }
};
