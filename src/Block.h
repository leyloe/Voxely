#ifndef VOXELY_BLOCK_H
#define VOXELY_BLOCK_H

#include <cstdint>

enum class BlockId : uint8_t {
    Air  = 0,
    Dirt = 1,
};

constexpr bool isSolid(const BlockId b) { return b != BlockId::Air; }

#endif //VOXELY_BLOCK_H