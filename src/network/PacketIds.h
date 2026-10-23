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

    // --- Только от Сервера к Клиенту (Clientbound) ---
    UpdateTime      = 0x04,
    EntityEquipment = 0x05,
    SpawnPosition   = 0x06,
    UpdateHealth    = 0x08,
    Respawn         = 0x09,
    HeldItemChange  = 0x10, // В клиенте это HandSlot
    UseBed          = 0x11,
    Animation       = 0x12,
    SpawnEntity     = 0x14,
    SpawnMob        = 0x18,
    EntityVelocity  = 0x1C,
    DestroyEntity   = 0x1D,
    Entity          = 0x1E,
    RelEntityMove   = 0x1F,
    EntityLook      = 0x20,
    RelEntityMoveLook = 0x21,
    EntityTeleport  = 0x22,
    EntityStatus    = 0x26,
    AttachEntity    = 0x27,
    EntityMetadata  = 0x28,
    ChunkState      = 0x32,
    ChunkData       = 0x33,
    MultiBlockChange = 0x34,
    BlockChange     = 0x35,
    PlayNoteBlock   = 0x36,
    Explosion       = 0x3C,
    OpenWindow      = 0x64,
    CloseWindow     = 0x65,
    SetSlot         = 0x67,
    WindowItems     = 0x68,
    UpdateProgressBar = 0x69,
    Transaction     = 0x6A,

    // --- Только от Клиента к Серверу (Serverbound) ---
    Flying          = 0x0A,
    Position        = 0x0B,
    Look            = 0x0C,
    PositionLook    = 0x0D,
    BreakBlock      = 0x0E,
    PlaceBlock      = 0x0F,
    BlockItemSwitch = 0x10, // Тот же ID, что и HeldItemChange
    EntityAction    = 0x13,
    WindowClick     = 0x66,

    // --- Общие для обоих (зависят от контекста) ---
    Disconnect      = 0xFF
};

// Вспомогательная функция для красивых логов
inline const char* getPacketName(PacketId id) {
    switch (id) {
        case PacketId::KeepAlive: return "KeepAlive";
        case PacketId::Login: return "Login";
        case PacketId::Handshake: return "Handshake";
        case PacketId::Chat: return "Chat";
        case PacketId::UpdateTime: return "UpdateTime";
        case PacketId::SpawnPosition: return "SpawnPosition";
        case PacketId::UpdateHealth: return "UpdateHealth";
        case PacketId::Respawn: return "Respawn";
        case PacketId::Flying: return "Flying";
        case PacketId::Position: return "Position";
        case PacketId::Look: return "Look";
        case PacketId::PositionLook: return "PositionLook";
        case PacketId::BreakBlock: return "BreakBlock";
        case PacketId::PlaceBlock: return "PlaceBlock";
        case PacketId::BlockItemSwitch: return "BlockItemSwitch";
        case PacketId::Animation: return "Animation";
        case PacketId::EntityAction: return "EntityAction";
        case PacketId::ChunkState: return "ChunkState";
        case PacketId::ChunkData: return "ChunkData";
        case PacketId::MultiBlockChange: return "MultiBlockChange";
        case PacketId::BlockChange: return "BlockChange";
        case PacketId::Disconnect: return "Disconnect";
        default: return "Unknown";
    }
}