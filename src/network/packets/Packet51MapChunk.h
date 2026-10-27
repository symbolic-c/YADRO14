#pragma once
#include <network/Packet.h>
#include <vector>
#include <zlib.h>
#include <iostream>
#include <random>

class Packet51MapChunk : public Packet {
public:
    int32_t x = 0;
    int16_t y = 0;
    int32_t z = 0;
    int8_t width = 15;      // 16 - 1
    int8_t height = 127;    // 128 - 1
    int8_t length = 15;     // 16 - 1

    // Сырые данные (блоки + метаданные + свет)
    std::vector<uint8_t> rawData;

    PacketId getId() const override { return PacketId::MapChunk; }

    // Чтение (клиент нам это не шлет, но метод нужен для интерфейса)
    void read(ByteBuffer& buffer) override {}

    void write(ByteBuffer& buffer) const override {
        // 1. Сжимаем данные через zlib
        uLongf compressedSize = compressBound(rawData.size());
        std::vector<uint8_t> compressedData(compressedSize);

        if (compress2(compressedData.data(), &compressedSize, rawData.data(), rawData.size(), Z_DEFAULT_COMPRESSION) != Z_OK) {
            std::cerr << "Ошибка сжатия чанка!" << std::endl;
            return;
        }
        compressedData.resize(compressedSize);

        // 2. Пишем заголовок пакета
        buffer.writeInt32(x);
        buffer.writeInt16(y);
        buffer.writeInt32(z);
        buffer.writeByte(width);
        buffer.writeByte(height);
        buffer.writeByte(length);
        buffer.writeInt32(static_cast<int32_t>(compressedSize));

        // 3. Дописываем сжатые данные
        buffer.data.insert(buffer.data.end(), compressedData.begin(), compressedData.end());

        std::cout << "[Сервер] Отправлен ChunkData: X=" << x << ", Z=" << z
                  << " (Сжато до " << compressedSize << " байт)" << std::endl;
    }

    void handle(Player& player, ClientSession& session) override {}
};