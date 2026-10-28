#pragma once
#include <network/Packet.h>

class Packet53BlockChange : public Packet {
public:
    int32_t x = 0;
    int8_t y = 0;
    int32_t z = 0;
    int8_t blockId = 0;
    int8_t metadata = 0;

    PacketId getId() const override { return static_cast<PacketId>(0x35); }

    void read(ByteBuffer& buffer) override {}
    void write(ByteBuffer& buffer) const override {
        buffer.writeInt32(x);
        buffer.writeByte(y);
        buffer.writeInt32(z);
        buffer.writeByte(blockId);
        buffer.writeByte(metadata);
    }
    void handle(Player& player, ClientSession& session) override {}
};