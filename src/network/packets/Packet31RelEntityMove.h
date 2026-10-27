#pragma once
#include <network/Packet.h>

class Packet31RelEntityMove : public Packet {
public:
    int32_t entityId;
    int8_t dx, dy, dz; // Смещение в 1/32 долях блока

    Packet31RelEntityMove() : entityId(0), dx(0), dy(0), dz(0) {}

    PacketId getId() const override { return static_cast<PacketId>(0x1F); }

    void read(ByteBuffer& buffer) override {}

    void write(ByteBuffer& buffer) const override {
        buffer.writeInt32(entityId);
        buffer.writeByte(dx);
        buffer.writeByte(dy);
        buffer.writeByte(dz);
    }

    void handle(Player& player, ClientSession& s) override {}
};