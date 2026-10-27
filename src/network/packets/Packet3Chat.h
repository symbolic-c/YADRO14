#pragma once
#include <network/Packet.h>

class Packet3Chat : public Packet {
public:
    std::string message;

    PacketId getId() const override { return PacketId::Chat; }

    void read(ByteBuffer &buffer) override {
        message = buffer.readString();
        if (message.length() > 100) {
            std::cerr << "Слишком длинное сообщение! Игрок будет кикнут." << std::endl;
            // (Тут нужно вызвать кик через сессию)
        }
    }

    void write(ByteBuffer &buffer) const override {
        buffer.writeString(message);
    }

    void handle(Player& player, ClientSession &session) override {
        Packet3Chat response;
        response.message = "<" + player.username + "> " + message;
        session.playerManager->broadcast(response, -1);
    }
};
