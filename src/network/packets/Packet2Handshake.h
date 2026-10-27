#pragma once
#include "../Packet.h"
#include <string>
#include <iostream>
#include <core/ClientSession.h>

class Packet2Handshake : public Packet {
public:
    std::string username;

    PacketId getId() const override { return PacketId::Handshake; }

    void read(ByteBuffer& buffer) override {
        username = buffer.readString();
    }

    void handle(Player& player, ClientSession& session) override {
        player.username = username;
        std::cout << "[Сервер] Handshake от " << username << std::endl;
        Packet2Handshake response;
        response.username = "-";
        session.sendPacket(response);
    }

    void write(ByteBuffer& buffer) const override {
        buffer.writeString("-");
    }
};
