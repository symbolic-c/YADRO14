#pragma once
#include <cstdint>

// Направление пакета
enum class PacketDirection {
    Clientbound, // Сервер -> Клиент
    Serverbound  // Клиент -> Сервер
};

enum class PacketId : uint8_t {
    // --- Базовые (оба направления) ---
    KeepAlive       = 0x00,
    Login           = 0x01,
    Handshake       = 0x02,
    Chat            = 0x03,
    Flying          = 0x0A,
    PlayerLook      = 0x0C,
    Animation       = 0x12,

    // --- Только от Сервера к Клиенту (Clientbound) ---

    UpdateTime      = 0x04,
    NamedEntitySpawn = 0x14,
    PickupSpawn     = 0x15,
    Collect         = 0x16,
    DestroyEntity   = 0x1D,
    RelEntityMove   = 0x1F,
    EntityLook      = 0x20,
    EntityTeleport  = 0x22,
    EntityMetadata  = 0x28,
    PreChunk        = 0x32,
    MapChunk        = 0x33,
    BlockChange     = 0x35,
    SetSlot         = 0x67,


    // --- Только от Клиента к Серверу (Serverbound) ---
    Position        = 0x0B,
    PlayerLookMove  = 0x0D,
    BlockDig        = 0x0E,
    Place           = 0x0F,
    EntityAction    = 0x13,


    // --- Общие для обоих (зависят от контекста) ---
    KickDisconnect      = 0xFF
};

// Вспомогательная функция для красивых логов
inline const char* getPacketName(PacketId id) {
    switch (id) {
        case PacketId::KeepAlive: return "KeepAlive";
        case PacketId::Login: return "Login";
        case PacketId::Handshake: return "Handshake";
        case PacketId::Chat: return "Chat";
        case PacketId::Flying: return "Flying";
        case PacketId::Position: return "Position";
        case PacketId::PlayerLook: return "PlayerLook";
        case PacketId::PlayerLookMove: return "PlayerLookMove";
        case PacketId::EntityAction: return "EntityAction";
        case PacketId::PreChunk: return "PreChunk";
        case PacketId::MapChunk: return "MapChunk";
        case PacketId::KickDisconnect: return "KickDisconnect";
        default: return "Unknown";
    }
}