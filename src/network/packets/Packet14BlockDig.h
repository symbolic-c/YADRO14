#pragma once
#include <network/Packet.h>

#include "Packet21PickupSpawn.h"
#include "Packet53BlockChange.h" // Понадобится для обновления
#include "world/World.h"

class Packet14BlockDig : public Packet {
public:
    int8_t status = 0;
    int32_t x = 0;
    int8_t y = 0;
    int32_t z = 0;
    int8_t face = 0;

    PacketId getId() const override { return PacketId::BlockDig; } // 0x0E

    void read(ByteBuffer& buffer) override {
        status = buffer.readByte();
        x = buffer.readInt32();
        y = buffer.readByte();
        z = buffer.readInt32();
        face = buffer.readByte();
    }
    void write(ByteBuffer& buffer) const override {
        buffer.writeByte(status);
        buffer.writeInt32(x);
        buffer.writeByte(y);
        buffer.writeInt32(z);
        buffer.writeByte(face);
    }

    void handle(Player& player, ClientSession& session) override {
        // В Beta 1.7.3 статус 2 означает "блок сломан"
        if (status == 2) {
            // 1. Получаем ID блока
            uint8_t blockId = session.world->getBlock(x, y, z);

            // Если это воздух, ничего не делаем
            if (blockId == 0) return;

            std::cout << "[Сервер] Игрок сломал блок ID: " << (int)blockId << std::endl;

            // 2. Обновляем мир
            session.world->setBlock(x, y, z, 0);
            std::cout << "[DEBUG] setBlock called!" << std::endl;

            // 3. Отправляем всем изменение блока (чтобы блок исчез у всех)
            Packet53BlockChange change;
            change.x = x; change.y = y; change.z = z;
            change.blockId = 0;
            change.metadata = 0;
            session.playerManager->broadcast(change, -1);

            // 4. Создаем выпавший предмет (ID блока = ID предмета)
            Packet21PickupSpawn spawn;
            spawn.entityId = rand() % 10000 + 1000; // Уникальный ID
            spawn.itemId = blockId; // Выпадает то, что сломали
            spawn.count = 1;
            spawn.damage = 0;

            // Координаты в квантах (умножаем на 32)
            spawn.x = (x * 32) + 16;
            spawn.y = (y * 32) + 16;
            spawn.z = (z * 32) + 16;

            // Небольшой подскок
            spawn.velX = 0;
            spawn.velY = 5;
            spawn.velZ = 0;

            // Сохраняем в мир, чтобы можно было подобрать
            session.world->spawnItem(spawn.entityId, spawn.itemId, x + 0.5, y + 0.5, z + 0.5);

            // Рассылаем пакет всем
            session.playerManager->broadcast(spawn, -1);
        }
    }
};