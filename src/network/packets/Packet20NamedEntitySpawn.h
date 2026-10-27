#pragma once


#include <network/Packet.h>
#include <string>

class Packet20NamedEntitySpawn : public Packet {
public:
    int32_t entityId;
    std::string name;
    int32_t x, y, z;       // Координаты, умноженные на 32
    int8_t rotation, pitch; // Углы (0-255)
    int16_t heldItem;       // <--- ДОБАВЛЕНО: ID предмета в руке (0 = пусто)

    Packet20NamedEntitySpawn() : heldItem(0) {} // По умолчанию 0 (пустая рука)

    PacketId getId() const override { return static_cast<PacketId>(0x14); }

    void read(ByteBuffer& b) override {}

    void write(ByteBuffer& b) const override {
        b.writeInt32(entityId);
        b.writeString(name);
        b.writeInt32(x);
        b.writeInt32(y);
        b.writeInt32(z);
        b.writeByte(rotation);
        b.writeByte(pitch);
        b.writeInt16(heldItem); // <--- ОБЯЗАТЕЛЬНО В КОНЦЕ!
    }

    void handle(Player& player, ClientSession& s) override {}
};
