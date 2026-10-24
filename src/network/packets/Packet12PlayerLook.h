#pragma once
#include <network/Packet.h>

class Packet12PlayerLook : public Packet {
public:
    float yaw = 0;
    float pitch = 0;
    bool onGround = false;

    Packet12PlayerLook() = default;

    PacketId getId() const override { return PacketId::PlayerLook; }

    void read(ByteBuffer& buffer) override {
        yaw = buffer.readFloat();
        pitch = buffer.readFloat();
        onGround = buffer.readBool();
    }

    void write(ByteBuffer& buffer) const override {
        buffer.writeFloat(yaw);
        buffer.writeFloat(pitch);
        buffer.writeBool(onGround);
    }

    void handle(ClientSession& session) override {
        // Обновляем состояние игрока в сессии
        session.yaw = yaw;
        session.pitch = pitch;
        session.onGround = onGround;
    }
};
