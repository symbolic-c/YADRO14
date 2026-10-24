#pragma once
#include <cstdint>
#include <network/Packet.h>
#include <network/PacketIds.h>
#include <iostream>
#include "Packet3Chat.h"

class Packet1Login : public Packet {
public:
    // Поля для ответа сервера
    int32_t protocolVersion = 14;
    int32_t entityId = 1;
    int64_t mapSeed = 0;
    int64_t seed = 0;
    int8_t dimension = 0;
    std::string username;

    PacketId getId() const override { return PacketId::Login; }

    void read(ByteBuffer& buffer) override {
        protocolVersion = buffer.readInt32();
        username = buffer.readString();
        mapSeed = buffer.readInt64();
        dimension = buffer.readByte();

        std::cout << "[Login] Protocol: " << protocolVersion
                  << ", User: " << username
                  << ", Seed: " << mapSeed
                  << ", Dim: " << (int)dimension << std::endl;
    }

    void write(ByteBuffer& buffer) const override {
        // Сервер отвечает: Entity ID (int), Username (String), Seed (long), Dimension (byte)
        buffer.writeInt32(entityId);
        buffer.writeString(username);
        buffer.writeInt64(seed);
        buffer.writeByte(dimension);
        std::cout << "[Login] Отправлен ответ на Login (Entity ID: " << entityId << ")" << std::endl;
    }

    void handle(ClientSession& session) override {
        session.username = username;
        std::cout << "[Сервер] Получен Login от " << username << std::endl;

        Packet3Chat chat_response;
        chat_response.message = username + " joined da game.";
        session.sendPacket(chat_response);

        Packet1Login response;
        response.protocolVersion = 14;
        response.username = username;
        response.mapSeed = 0;
        response.dimension = 0;
        session.sendPacket(response);
        session.isLoggedIn = true;


        std::cout << "[Сервер] Отправлен Login." << std::endl;
    }
};
