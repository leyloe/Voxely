#include "Block.h"

namespace {
    constexpr dm::float3 RAW_FACES[static_cast<int>(Face::Count)][VERTICES_PER_FACE] = {
        { {-0.5f, 0.5f, 0.5f}, { 0.5f, 0.5f, 0.5f}, { 0.5f,-0.5f, 0.5f}, {-0.5f,-0.5f, 0.5f} },
        { { 0.5f, 0.5f,-0.5f}, {-0.5f, 0.5f,-0.5f}, {-0.5f,-0.5f,-0.5f}, { 0.5f,-0.5f,-0.5f} },
        { {-0.5f, 0.5f,-0.5f}, {-0.5f, 0.5f, 0.5f}, {-0.5f,-0.5f, 0.5f}, {-0.5f,-0.5f,-0.5f} },
        { { 0.5f, 0.5f, 0.5f}, { 0.5f, 0.5f,-0.5f}, { 0.5f,-0.5f,-0.5f}, { 0.5f,-0.5f, 0.5f} },
        { {-0.5f, 0.5f,-0.5f}, { 0.5f, 0.5f,-0.5f}, { 0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f} },
        { {-0.5f,-0.5f, 0.5f}, { 0.5f,-0.5f, 0.5f}, { 0.5f,-0.5f,-0.5f}, {-0.5f,-0.5f,-0.5f} },
    };

    constexpr dm::float2 DIRT_UV[VERTICES_PER_FACE] = {
        {0.f, 1.f}, {1.f, 1.f}, {1.f, 0.f}, {0.f, 0.f},
    };
}

bool Block::isSolid(const BlockId b) { return b != BlockId::Air; }

Block::Block(const dm::float3 position) : m_Position(position) {}

dm::float3 Block::GetPosition() const { return m_Position; }

FaceData Block::GetFaceData(const Face face) const {
    FaceData data;
    const int f = static_cast<int>(face);
    for (int i = 0; i < VERTICES_PER_FACE; ++i) {
        data.vertices[i] = m_Position + RAW_FACES[f][i];
        data.uv[i]       = DIRT_UV[i];
    }
    return data;
}


