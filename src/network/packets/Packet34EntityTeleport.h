#pragma once
#include <network/Packet.h>

class Packet34EntityTeleport : public Packet {
public:
    int32_t entityId;
    int32_t x, y, z; // Координаты, умноженные на 32
    int8_t yaw, pitch; // Углы (0-255)

    Packet34EntityTeleport() : entityId(0), x(0), y(0), z(0), yaw(0), pitch(0) {}

    PacketId getId() const override { return static_cast<PacketId>(0x22); }

    void read(ByteBuffer& b) override {}

    void write(ByteBuffer& b) const override {
        b.writeInt32(entityId);
        b.writeInt32(x);
        b.writeInt32(y);
        b.writeInt32(z);
        b.writeByte(yaw);
        b.writeByte(pitch);
    }

    void handle(ClientSession& s) override {}
};