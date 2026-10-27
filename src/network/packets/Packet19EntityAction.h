#pragma once
#include <network/Packet.h>


class Packet19EntityAction : public Packet {
public:
    int32_t entityId = 0;
    int8_t actionId = 0;

    PacketId getId() const override { return PacketId::EntityAction; }

    void read(ByteBuffer &buffer) override {
        entityId = buffer.readInt32();
        actionId = buffer.readByte();

        // Расшифровка действий для отладки
        if (actionId == 1) std::cout << "[Сервер] Игрок начал красться." << std::endl;
        else if (actionId == 2) std::cout << "[Сервер] Игрок перестал красться." << std::endl;
        else if (actionId == 3) std::cout << "[Сервер] Игрок перестал спать." << std::endl;
    }

    void write(ByteBuffer &buffer) const override {
        buffer.writeInt32(entityId);
        buffer.writeByte(actionId);
    }

    void handle(Player& player, ClientSession &session) override {}
};
