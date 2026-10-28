#pragma once
#include <network/Packet.h>

class Packet21PickupSpawn : public Packet {
public:
    int32_t entityId = 0;
    int16_t itemId = 0;
    int8_t count = 1;
    int16_t damage = 0;

    // Координаты (умноженные на 32)
    int32_t x = 0, y = 0, z = 0;

    // Скорость (умноженная на 128)
    int8_t velX = 0, velY = 0, velZ = 0;

    PacketId getId() const override { return static_cast<PacketId>(0x15); }

    void read(ByteBuffer& buffer) override {}

    void write(ByteBuffer& buffer) const override {
        buffer.writeInt32(entityId);
        buffer.writeInt16(itemId);
        buffer.writeByte(count);
        buffer.writeInt16(damage);
        buffer.writeInt32(x);
        buffer.writeInt32(y);
        buffer.writeInt32(z);
        buffer.writeByte(velX);
        buffer.writeByte(velY);
        buffer.writeByte(velZ);
    }

    void handle(Player& player, ClientSession& session) override {}
};