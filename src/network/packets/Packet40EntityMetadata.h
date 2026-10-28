#pragma once
#include <network/Packet.h>
#include <vector>

class Packet40EntityMetadata : public Packet {
public:
    int32_t entityId;
    std::vector<uint8_t> metadata;

    Packet40EntityMetadata() : entityId(0) {}

    PacketId getId() const override { return static_cast<PacketId>(0x28); }

    void read(ByteBuffer& buffer) override {}
    
    void write(ByteBuffer& buffer) const override {
        buffer.writeInt32(entityId);
        
        // Записываем сами метаданные
        for (uint8_t byte : metadata) {
            buffer.writeByte(byte);
        }
        
        // Обязательный маркер конца метаданных
        buffer.writeByte(0x7F); 
    }

    void handle(Player& player, ClientSession& session) override {}

    // Вспомогательный метод для приседания
    void setSneaking(bool isSneaking) {
        metadata.clear();
        metadata.push_back(0x00);
        metadata.push_back(isSneaking ? 0x02 : 0x00); // 0x02 = присед, 0x00 = стоит
    }
};