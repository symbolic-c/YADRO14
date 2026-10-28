#pragma once
#include <network/Packet.h>

#include "Packet103SetSlot.h"
#include "Packet53BlockChange.h"

class Packet15Place : public Packet {
public:
    int32_t x = 0;
    int8_t y = 0;
    int32_t z = 0;
    int8_t face = 0;
    int16_t itemId = -1;
    int8_t itemCount = 0;
    int16_t itemDamage = 0;

    PacketId getId() const override { return static_cast<PacketId>(0x0F); }

    void read(ByteBuffer& buffer) override {
        x = buffer.readInt32();
        y = buffer.readByte();
        z = buffer.readInt32();
        face = buffer.readByte();
        itemId = buffer.readInt16();

        // ВСЕГДА читаем эти поля, даже если itemId == -1
        itemCount = buffer.readByte();
        itemDamage = buffer.readInt16();

        if (itemId >= 0) {
            itemCount = buffer.readByte();
            itemDamage = buffer.readInt16();
        } else {
            itemCount = 0;
            itemDamage = 0;
        }
    }
    void write(ByteBuffer& buffer) const override {
        buffer.writeInt32(x);
        buffer.writeByte(y);
        buffer.writeInt32(z);
        buffer.writeByte(face);
        buffer.writeInt16(itemId);

        // Всегда пишем
        buffer.writeByte(itemCount);
        buffer.writeInt16(itemDamage);
    }

    void handle(Player& player, ClientSession& session) override {
        if (itemId < 0 || itemId >= 256) return;

        // 2. Вычисляем координаты НОВОГО блока
        int32_t targetX = x;
        int32_t targetY = y;
        int32_t targetZ = z;

        if (face == 0) targetY -= 1;
        else if (face == 1) targetY += 1;
        else if (face == 2) targetZ -= 1;
        else if (face == 3) targetZ += 1;
        else if (face == 4) targetX -= 1;
        else if (face == 5) targetX += 1;

        if (targetY < 0 || targetY > 127) return;

        // 3. Проверяем, что место свободно
        if (session.world->getBlock(targetX, targetY, targetZ) != 0) return;

        // 4. Сохраняем блок в мир
        session.world->setBlock(targetX, targetY, targetZ, static_cast<uint8_t>(itemId));

        // 5. Отправляем обновление блока всем
        Packet53BlockChange change;
        change.x = targetX;
        change.y = static_cast<int8_t>(targetY);
        change.z = targetZ;
        change.blockId = static_cast<int8_t>(itemId);
        change.metadata = 0;
        session.playerManager->broadcast(change, -1);

        // --- 6. ОБНОВЛЕНИЕ ИНВЕНТАРЯ (Чтобы блок не исчезал) ---
        // Ищем в инвентаре слот с этим предметом, чтобы уменьшить количество
        for (int i = 0; i < 36; ++i) {
            if (session.player->inventory[i].id == itemId) {
                session.player->inventory[i].count--;

                // Если блоки закончились, очищаем слот (-1)
                if (session.player->inventory[i].count <= 0) {
                    session.player->inventory[i].id = -1;
                    session.player->inventory[i].count = 0;
                }

                // Отправляем обновление слота клиенту (windowId=0, слот с учетом хотбара)
                Packet103SetSlot slotPacket;
                slotPacket.windowId = 0;
                // Переводим индекс в сетевой (0-8 -> 36-44, 9-35 -> 9-35)
                slotPacket.slot = (i <= 8) ? (i + 36) : i;
                slotPacket.item = session.player->inventory[i];
                session.sendPacket(slotPacket);
                break;
            }
        }

        std::cout << "[Сервер] Блок ID " << itemId << " поставлен на ("
                  << targetX << ", " << targetY << ", " << targetZ << ")" << std::endl;
    }
};