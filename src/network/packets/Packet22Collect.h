#pragma once
#include <network/Packet.h>
#include "Packet29DestroyEntity.h" // Пакет для удаления сущности

class Packet22Collect : public Packet {
public:
    int32_t collectedEntityId = 0; // ID предмета, который подобрали
    int32_t collectorEntityId = 0; // ID того, кто подобрал

    PacketId getId() const override { return static_cast<PacketId>(0x16); }

    void read(ByteBuffer& buffer) override {
        collectedEntityId = buffer.readInt32();
        collectorEntityId = buffer.readInt32();
    }
    void write(ByteBuffer& buffer) const override {
        buffer.writeInt32(collectedEntityId);
        buffer.writeInt32(collectorEntityId);
    }

    void handle(Player& player, ClientSession& session) override {
        // 1. Удаляем предмет из мира у всех игроков
        Packet29DestroyEntity destroy;
        destroy.entityId = collectedEntityId;
        session.playerManager->broadcast(destroy, -1);

        // 2. (Опционально) Добавляем предмет в инвентарь игрока
        // Для этого вам понадобится система инвентаря и пакет Packet103SetSlot
        std::cout << "[Сервер] Игрок " << player.username
                  << " подобрал предмет ID " << collectedEntityId << std::endl;
    }
};