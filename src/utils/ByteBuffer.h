#pragma once
#include <vector>
#include <cstdint>
#include <cstring>
#include <string>

class ByteBuffer {
public:
    std::vector<uint8_t> data;
    size_t readOffset = 0;

    ByteBuffer() = default;
    explicit ByteBuffer(std::vector<uint8_t> raw) : data(std::move(raw)) {}

    // --- Запись (Big-Endian) ---
    void writeByte(uint8_t v) { data.push_back(v); }
    void writeInt16(int16_t v) { data.push_back((v>>8)&0xFF); data.push_back(v&0xFF); }
    void writeInt32(int32_t v) {
        data.push_back((v>>24)&0xFF); data.push_back((v>>16)&0xFF);
        data.push_back((v>>8)&0xFF);  data.push_back(v&0xFF);
    }
    void writeInt64(int64_t v) { for (int i=7;i>=0;--i) data.push_back((v>>(i*8))&0xFF); }
    void writeFloat(float v) { uint32_t u; memcpy(&u,&v,4); writeInt32((int32_t)u); }
    void writeDouble(double v) { uint64_t u; memcpy(&u,&v,8); writeInt64((int64_t)u); }
    void writeBool(bool v) { writeByte(v?1:0); }
    void writeString(const std::string& str) {
        writeInt16(static_cast<int16_t>(str.length()));
        for (char c : str) { writeByte(0x00); writeByte(c); }
    }

    // --- Чтение (Big-Endian) ---
    uint8_t readByte() { return data[readOffset++]; }
    int16_t readInt16() {
        int16_t v = (data[readOffset] << 8) | data[readOffset+1];
        readOffset += 2; return v;
    }
    int32_t readInt32() {
        int32_t v = (data[readOffset] << 24) | (data[readOffset+1] << 16) |
                    (data[readOffset+2] << 8) | data[readOffset+3];
        readOffset += 4; return v;
    }
    int64_t readInt64() {
        int64_t v = 0;
        for (int i = 0; i < 8; ++i) v = (v << 8) | data[readOffset++];
        return v;
    }
    float readFloat() { int32_t i = readInt32(); float f; memcpy(&f, &i, 4); return f; }
    double readDouble() { int64_t i = readInt64(); double d; memcpy(&d, &i, 8); return d; }
    bool readBool() { return readByte() != 0; }
    std::string readString() {
        int16_t len = readInt16();
        std::string str;
        for (int i = 0; i < len; ++i) {
            readByte(); // Пропускаем нулевой байт UTF-16BE
            str += static_cast<char>(readByte());
        }
        return str;
    }
};