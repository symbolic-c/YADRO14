#include "World.h"

#include <algorithm>

uint8_t World::getBlock(int x, int y, int z) {
    std::lock_guard<std::mutex> lock(worldMutex);
    auto key = std::make_tuple(x, y, z);

    // 1. Если блок был изменен игроками - возвращаем измененное значение
    if (blocks.find(key) != blocks.end()) {
        return blocks[key];
    }

    // 2. Если блока нет в map, генерируем его "на лету" (как FlatWorldGenerator)
    if (y == 0) return 0x07;      // Бедрок
    if (y == 1 || y == 2) return 0x03; // Земля
    if (y == 3) return 0x02;      // Трава

    return 0x00; // Воздух по умолчанию
}

void World::setBlock(int x, int y, int z, uint8_t blockId) {
    std::lock_guard<std::mutex> lock(worldMutex);
    auto key = std::make_tuple(x, y, z);
    blocks[key] = blockId;
}

void World::spawnItem(int32_t entityId, int16_t itemId, double x, double y, double z) {
    std::lock_guard<std::mutex> lock(worldMutex);
    droppedItems.push_back({entityId, itemId, 1, x, y, z});
}

std::vector<DroppedItem> World::getDroppedItems() {
    std::lock_guard<std::mutex> lock(worldMutex);
    return droppedItems; // Возвращаем копию
}

void World::removeItem(int32_t entityId) {
    std::lock_guard<std::mutex> lock(worldMutex);
    droppedItems.erase(std::remove_if(droppedItems.begin(), droppedItems.end(),
        [entityId](const DroppedItem& item) { return item.entityId == entityId; }),
        droppedItems.end());
}