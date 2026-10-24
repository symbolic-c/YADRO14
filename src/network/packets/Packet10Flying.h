#pragma once
#include <network/Packet.h>

class Packet10Flying : public Packet {
public:
    bool onGround = false;
    PacketId getId() const override { return PacketId::Flying; }

    void read(ByteBuffer &buffer) override {
        onGround = buffer.readBool();
    }

    void write(ByteBuffer &buffer) const override {
        buffer.writeBool(onGround);
    }

    void handle(ClientSession &session) override {}
};
