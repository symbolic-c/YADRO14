#pragma once
#include <map>
#include <vector>
#include <cstdint>
#include <mutex>

// Структура для хранения выпавшего предмета
struct DroppedItem {
    int32_t entityId;
    int16_t itemId;
    int8_t count;
    double x, y, z;
};

class Chunk; // Предварительное объявление

class World {
public:
    // Получить/изменить блок
    uint8_t getBlock(int x, int y, int z);
    void setBlock(int x, int y, int z, uint8_t blockId);

    // Управление предметами
    void spawnItem(int32_t entityId, int16_t itemId, double x, double y, double z);
    std::vector<DroppedItem> getDroppedItems();
    void removeItem(int32_t entityId);

    std::vector<uint8_t> generateChunkData(int chunkX, int chunkZ);

private:
    // Для начала можно использовать std::map с ключом по координатам
    // В идеале - своя реализация чанков (16x16x128)
    std::map<std::tuple<int, int, int>, uint8_t> blocks;
    
    std::vector<DroppedItem> droppedItems;
    std::mutex worldMutex; // Защита от одновременного доступа из разных потоков
};