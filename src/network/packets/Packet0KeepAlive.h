#pragma once
#include "../Packet.h"
#include <iostream>

class Packet0KeepAlive : public Packet {
public:
    Packet0KeepAlive() = default;

    PacketId getId() const override { return PacketId::KeepAlive; }

    void read(ByteBuffer& buffer) override {
        // В Beta 1.7.3 пакет KeepAlive не содержит данных, только ID.
        // Ничего не читаем.
    }

    void write(ByteBuffer& buffer) const override {
        // KeepAlive не содержит данных.
    }

    void handle(ClientSession &session) override {
        session.sendPacket(*this);
    }
};