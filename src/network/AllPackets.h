#pragma once
#include <network/PacketFactory.h>
#include <network/packets/Packet2Handshake.h>
#include <network/packets/Packet1Login.h>

#include "packets/Packet0KeepAlive.h"
#include "packets/Packet11Position.h"
#include "packets/Packet13PlayerLookMove.h"


inline void registerAllPackets() {
    PacketFactory::registerPacket(PacketId::KeepAlive, []() { return std::make_shared<Packet0KeepAlive>(); });
    PacketFactory::registerPacket(PacketId::Login, []() { return std::make_shared<Packet1Login>(); });
    PacketFactory::registerPacket(PacketId::Handshake, []() { return std::make_shared<Packet2Handshake>(); });
    PacketFactory::registerPacket(PacketId::Position, []() { return std::make_shared<Packet11Position>(); });
    PacketFactory::registerPacket(PacketId::PositionLook, []() { return std::make_shared<Packet13PlayerLookMove>(); });
}
