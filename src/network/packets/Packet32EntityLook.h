#pragma once
#include <network/Packet.h>

class Packet32EntityLook : public Packet {
public:
    int32_t entityId;
    int8_t yaw;
    int8_t pitch;

    Packet32EntityLook() : entityId(0), yaw(0), pitch(0) {}

    PacketId getId() const override { return static_cast<PacketId>(0x20); }

    void read(ByteBuffer& buffer) override {}

    void write(ByteBuffer& buffer) const override {
        buffer.writeInt32(entityId);
        buffer.writeByte(yaw);
        buffer.writeByte(pitch);
    }

    void handle(Player& player, ClientSession& session) override {
        
    }
};