#pragma once
#include <vector>
#include <cstdint>

class FlatWorldGenerator {
public:
    // Генерирует сырые данные для одного чанка (плоский мир)
    static std::vector<uint8_t> generateChunk();
};