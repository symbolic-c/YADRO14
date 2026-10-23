#pragma once
#include "PacketFactory.h"
#include "packets/Packet2Handshake.h"

inline void registerAllPackets() {
    PacketFactory::registerPacket(PacketId::Handshake, []() { return std::make_shared<Packet2Handshake>(); });
}