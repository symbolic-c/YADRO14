#pragma once

#include <iostream>
#include <network/Packet.h>

class Packet50PreChunk : public Packet {
public:
    int x = 0, z = 0;
    bool load = true;
    PacketId getId() const override { return PacketId::PreChunk; }

    void read(ByteBuffer &buffer) override {}

    void write(ByteBuffer &buffer) const override {
        buffer.writeInt32(x);
        buffer.writeInt32(z);
        buffer.writeBool(load);
        std::cout << "[Сервер] Отправлен PreChunk: X=" << x << ", Z=" << z << ", Load=" << load << std::endl;
    }

    void handle(Player& player, ClientSession &session) override {}
};
