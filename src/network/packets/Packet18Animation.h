#pragma once
#include <network/Packet.h>

class Packet18Animation : public Packet {
public:
    int32_t entityId = 0;
    int8_t animate = 0;

    PacketId getId() const override { return static_cast<PacketId>(0x12); }

    void read(ByteBuffer& buffer) override {
        entityId = buffer.readInt32();
        animate = buffer.readByte();
    }
    void write(ByteBuffer& buffer) const override {
        buffer.writeInt32(entityId);
        buffer.writeByte(animate);
    }

    void handle(Player& player, ClientSession& session) override {
        // Рассылаем анимацию всем, кроме себя
        session.playerManager->broadcast(*this, player.entityId);
    }
};