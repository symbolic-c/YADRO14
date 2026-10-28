#pragma once
#include <network/Packet.h>
#include <item/ItemStack.h>

class Packet103SetSlot : public Packet {
public:
    int8_t windowId = 0;   // 0 = инвентарь игрока
    int16_t slot = 0;      // Слот (0-44 для инвентаря игрока)
    ItemStack item;        // Предмет

    Packet103SetSlot() = default;

    PacketId getId() const override { return static_cast<PacketId>(0x67); }

    void read(ByteBuffer& buffer) override {}
    
    void write(ByteBuffer& buffer) const override {
        buffer.writeByte(windowId);
        buffer.writeInt16(slot);
        buffer.writeInt16(item.id); // -1 если пусто
        
        // Item Amount и Item Metadata пишутся ТОЛЬКО если ID > -1
        if (item.id >= 0) {
            buffer.writeByte(item.count);
            buffer.writeInt16(item.damage);
        }
    }

    void handle(Player& player, ClientSession& session) override {}
};