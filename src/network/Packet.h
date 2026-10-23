#pragma once
#include <utils/ByteBuffer.h>
#include <memory>

#include "PacketIds.h"

class ClientSession;

class Packet {
public:
    virtual ~Packet() = default;
    virtual PacketId getId() const = 0;

    virtual void read(ByteBuffer& buffer) = 0;
    virtual void write(ByteBuffer& buffer) const = 0;
    virtual void handle(ClientSession& session) {}
};