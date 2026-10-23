#pragma once
#include <cstdint>
#include <network/Packet.h>
#include <network/PacketIds.h>
#include <iostream>

class Packet1Login : public Packet {
public:
    // Поля для ответа сервера
    int32_t entityId = 1;
    int64_t seed = 0;
    int8_t dimension = 0;
    std::string username;

    PacketId getId() const override { return PacketId::Login; }

    void read(ByteBuffer& buffer) override {
        // Клиент присылает: Protocol Version (int), Username (String), Map Seed (long), Dimension (byte)
        // Для простоты мы можем просто прочитать никнейм, так как версия протокола нам не важна
        int32_t protocolVersion = buffer.readInt32();
        username = buffer.readString();
        // Остальные данные (seed и dimension) клиент тоже шлет, но мы их игнорируем
        std::cout << "[Login] Клиент прислал логин. Ник: " << username << std::endl;
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
        std::cout << "[Сервер] Получен Login от " << username << std::endl;

        Packet1Login response;
        response.entityId = 1;
        response.username = username;
        response.seed = 0;
        response.dimension = 0;
        session.sendPacket(response);
    }
};