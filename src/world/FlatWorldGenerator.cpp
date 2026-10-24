#include "FlatWorldGenerator.h"

std::vector<uint8_t> FlatWorldGenerator::generateChunk() {
    std::vector<uint8_t> data;
    data.reserve(82176);

    // 1. Блоки (порядок важен: X -> Z -> Y)
    for (int x = 0; x < 16; ++x) {
        for (int z = 0; z < 16; ++z) {
            for (int y = 0; y < 128; ++y) {
                if (y == 0) {
                    data.push_back(0x07);      // 1 слой бедрока
                } else if (y == 1 || y == 2) {
                    data.push_back(0x03);      // 2 слоя земли
                } else if (y == 3) {
                    data.push_back(0x02);      // 1 слой травы
                } else {
                    data.push_back(0x00);      // Воздух
                }
            }
        }
    }

    // 2. Метаданные (нули)
    for (int i = 0; i < 16384; ++i) data.push_back(0x00);

    // 3. Свет блоков (нули)
    for (int i = 0; i < 16384; ++i) data.push_back(0x00);

    // 4. Небесный свет (0xFF = максимальный свет)
    for (int i = 0; i < 16384; ++i) data.push_back(0xFF);

    // 5. Биомы (1 = Plains / Равнины)
    for (int i = 0; i < 256; ++i) data.push_back(0x01);

    return data;
}