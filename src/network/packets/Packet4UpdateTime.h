#pragma once
#include <network/Packet.h>

class Packet4UpdateTime : public Packet {
public:
    // Время в тиках (long = 8 байт)
    int64_t time = 0;

    Packet4UpdateTime() = default;
    explicit Packet4UpdateTime(int64_t t) : time(t) {}

    PacketId getId() const override { return static_cast<PacketId>(0x04); }

    void read(ByteBuffer& buffer) override {
        time = buffer.readInt64();
    }

    void write(ByteBuffer& buffer) const override {
        buffer.writeInt64(time);
    }

    void handle(Player& player, ClientSession& session) override {
        // Клиент не присылает этот пакет серверу, 
        // только сервер отправляет его клиенту.
    }
};