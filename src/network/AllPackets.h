#pragma once
#include <network/PacketFactory.h>
#include <network/packets/Packet2Handshake.h>
#include <network/packets/Packet1Login.h>

#include "packets/Packet0KeepAlive.h"
#include "packets/Packet10Flying.h"
#include "packets/Packet11Position.h"
#include "packets/Packet12PlayerLook.h"
#include "packets/Packet13PlayerLookMove.h"
#include "packets/Packet19EntityAction.h"
#include "packets/Packet20NamedEntitySpawn.h"
#include "packets/Packet255KickDisconnect.h"
#include "packets/Packet29DestroyEntity.h"
#include "packets/Packet34EntityTeleport.h"
#include "packets/Packet3Chat.h"
#include "packets/Packet50PreChunk.h"
#include "packets/Packet51MapChunk.h"


inline void registerAllPackets() {
    PacketFactory::registerPacket(PacketId::KeepAlive, []() { return std::make_shared<Packet0KeepAlive>(); });
    PacketFactory::registerPacket(PacketId::Login, []() { return std::make_shared<Packet1Login>(); });
    PacketFactory::registerPacket(PacketId::Handshake, []() { return std::make_shared<Packet2Handshake>(); });
    PacketFactory::registerPacket(PacketId::Chat, []() { return std::make_shared<Packet3Chat>(); });
    PacketFactory::registerPacket(PacketId::Flying, []() { return std::make_shared<Packet10Flying>(); });
    PacketFactory::registerPacket(PacketId::Position, []() { return std::make_shared<Packet11Position>(); });
    PacketFactory::registerPacket(PacketId::PlayerLook, []() { return std::make_shared<Packet12PlayerLook>(); });
    PacketFactory::registerPacket(PacketId::PlayerLookMove, []() { return std::make_shared<Packet13PlayerLookMove>(); });
    PacketFactory::registerPacket(PacketId::EntityAction, []() { return std::make_shared<Packet19EntityAction>(); });
    PacketFactory::registerPacket(PacketId::NamedEntitySpawn, []() { return std::make_shared<Packet20NamedEntitySpawn>(); });
    PacketFactory::registerPacket(PacketId::DestroyEntity, []() { return std::make_shared<Packet29DestroyEntity>(); });
    PacketFactory::registerPacket(PacketId::EntityTeleport, []() { return std::make_shared<Packet34EntityTeleport>(); });
    PacketFactory::registerPacket(PacketId::PreChunk, []() { return std::make_shared<Packet50PreChunk>(); });
    PacketFactory::registerPacket(PacketId::MapChunk, []() { return std::make_shared<Packet51MapChunk>(); });
    PacketFactory::registerPacket(PacketId::KickDisconnect, []() { return std::make_shared<Packet255KickDisconnect>(); });
}
