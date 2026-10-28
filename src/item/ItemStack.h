#pragma once
#include <cstdint>

struct ItemStack {
    int16_t id = -1;     // -1 означает пустой слот
    int8_t count = 0;
    int16_t damage = 0;

    bool isEmpty() const { return id == -1 || count == 0; }
};