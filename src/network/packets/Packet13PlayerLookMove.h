#pragma once
#include <network/Packet.h>

class Packet13PlayerLookMove : public Packet {
public:
    double x = 0, y = 0, stance = 0, z = 0;
    float yaw = 0, pitch = 0;
    bool onGround = false;

    Packet13PlayerLookMove() = default;

    PacketId getId() const override { return PacketId::PlayerLookMove; }

    void read(ByteBuffer& buffer) override {
        x = buffer.readDouble();
        y = buffer.readDouble();
        stance = buffer.readDouble();
        z = buffer.readDouble();
        yaw = buffer.readFloat();
        pitch = buffer.readFloat();
        onGround = buffer.readBool();
    }

    void write(ByteBuffer& buffer) const override {
        buffer.writeDouble(x);
        buffer.writeDouble(y);
        buffer.writeDouble(stance);
        buffer.writeDouble(z);
        buffer.writeFloat(yaw);
        buffer.writeFloat(pitch);
        buffer.writeBool(onGround);
    }

    void handle(ClientSession& session) override {
        session.x = x;
        session.y = y;
        session.z = z;
    }
};