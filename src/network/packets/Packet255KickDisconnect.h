#pragma once
#include <iostream>
#include <network/Packet.h>

class Packet255KickDisconnect : public Packet{
public:
    std::string reason;
    PacketId getId() const override { return PacketId::KickDisconnect; }

    void read(ByteBuffer &buffer) override {
        reason = buffer.readString();
    }

    void write(ByteBuffer &buffer) const override {
        buffer.writeString(reason);
    }

    void handle(ClientSession &session) override {
        Packet3Chat chat_response;
        chat_response.message = session.username + " left da game.";
        session.sendPacket(chat_response);

        std::cout << "[Сервер] Клиент отключился. Причина: '" << reason << "'" << std::endl;
    }
};
