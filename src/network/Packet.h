#pragma once
#include "../utils/ByteBuffer.h"
#include <memory>

class ClientSession;

class Packet {
public:
    virtual ~Packet() = default;
    virtual uint8_t getId() const = 0;

    virtual void read(ByteBuffer& buffer) = 0;

    virtual void handle(ClientSession& session) {}
};