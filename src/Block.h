#ifndef VOXELY_BLOCK_H
#define VOXELY_BLOCK_H

#include <donut/core/math/math.h>

inline constexpr int VERTICES_PER_FACE = 4;

enum class BlockId : uint8_t {
    Air  = 0,
    Dirt = 1,
};

enum class Face : uint8_t {
    Front, Back, Left, Right, Top, Bottom, Count
};

struct FaceData {
    dm::float3 vertices[VERTICES_PER_FACE];
    dm::float2 uv[VERTICES_PER_FACE];
};


class Block {
public:
    Block() = default;

    static bool isSolid(BlockId b);

    explicit Block(dm::float3 position);
    [[nodiscard]] FaceData   GetFaceData(Face face) const;

    [[nodiscard]] dm::float3 GetPosition() const;

private:
    dm::float3 m_Position{ 0.f, 0.f, 0.f };
};

#endif //VOXELY_BLOCK_H