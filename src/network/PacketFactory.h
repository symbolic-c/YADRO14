#pragma once
#include "Packet.h"
#include <unordered_map>
#include <functional>

#include "PacketIds.h"

class PacketFactory {
public:
    using CreatorFunc = std::function<std::shared_ptr<Packet>()>;

    static void registerPacket(PacketId id, CreatorFunc creator) {
        getRegistry()[static_cast<uint8_t>(id)] = creator;
    }

    static std::shared_ptr<Packet> createPacket(uint8_t id) {
        auto it = getRegistry().find(id);
        if (it != getRegistry().end()) return it->second();
        return nullptr;
    }

private:
    static std::unordered_map<uint8_t, CreatorFunc>& getRegistry() {
        static std::unordered_map<uint8_t, CreatorFunc> registry;
        return registry;
    }
};
