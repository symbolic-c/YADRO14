#pragma once
#include <network/Packet.h>

class Packet29DestroyEntity : public Packet {
public:
    int32_t entityId;

    PacketId getId() const override { return static_cast<PacketId>(29); }
    void read(ByteBuffer& b) override {}
    void write(ByteBuffer& b) const override {
        b.writeInt32(entityId);
    }
    void handle(Player& player, ClientSession& s) override {}
};