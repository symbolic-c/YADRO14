#pragma once
#include <network/Packet.h>

class Packet11Position : public Packet {
public:
    double x = 0, y = 0, stance = 0, z = 0;
    bool onGround = false;

    Packet11Position() = default;

    PacketId getId() const override { return PacketId::Position; }

    void read(ByteBuffer& buffer) override {
        x = buffer.readDouble();
        y = buffer.readDouble();
        stance = buffer.readDouble();
        z = buffer.readDouble();
        onGround = buffer.readBool();
    }

    void write(ByteBuffer& buffer) const override {
        buffer.writeDouble(x);
        buffer.writeDouble(y);
        buffer.writeDouble(stance);
        buffer.writeDouble(z);
        buffer.writeBool(onGround);
    }

    void handle(ClientSession& session) override {
        session.x = x;
        session.y = y;
        session.z = z;
    }
};