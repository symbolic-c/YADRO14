#pragma once
#include "../Packet.h"
#include <string>
#include <iostream>

class Packet2Handshake : public Packet {
public:
    std::string username;

    uint8_t getId() const override { return 0x02; }

    void read(ByteBuffer& buffer) override {
        username = buffer.readString();
    }

    void handle(ClientSession &session) override {
        std:: cout << "Ник : " << username << std::endl;
    }
};