#include "Chunk.h"
#include "FastNoiseLite.h"

namespace {
    constexpr int FACE_NORMALS[6][3] = {
        {  0,  0,  1 },
        {  0,  0, -1 },
        { -1,  0,  0 },
        {  1,  0,  0 },
        {  0,  1,  0 },
        {  0, -1,  0 },
    };
}

Chunk::Chunk(const donut::math::float3 pos, const BlockId blockID) : m_Position(pos), m_BlockID(blockID) {}

int Chunk::Index(const int x, const int y, const int z) {
    return x + y * SIZE + z * SIZE * HEIGHT;
}

bool Chunk::IsSolidAt(const int x, const int y, const int z) const {
    if (x < 0 || y < 0 || z < 0 || x >= SIZE || y >= HEIGHT || z >= SIZE) return false;
    return Block::isSolid(m_ChunkBlocks[Index(x, y, z)]);
}

void Chunk::Generate() {
    for (int x = 0; x < SIZE; x++)
        for (int z = 0; z < SIZE; z++) {
            constexpr int   centerHeight = 8;
            constexpr float amplitude    = 6;

            const float n = m_heightMap[x + z * SIZE];
            const int columnHeight = centerHeight + static_cast<int>(n * amplitude);

            for (int y = 0; y < columnHeight && y < HEIGHT; y++)
                m_ChunkBlocks[Index(x, y, z)] = BlockId::Dirt;
        }

    for (int x = 0; x < SIZE; x++)
        for (int y = 0; y < HEIGHT; y++)
            for (int z = 0; z < SIZE; z++) {
                if (!Block::isSolid(m_ChunkBlocks[Index(x, y, z)])) continue;

                const Block block({static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)});
                for (int f = 0; f < static_cast<int>(Face::Count); f++) {
                    if (const int* nrm = FACE_NORMALS[f]; IsSolidAt(x + nrm[0], y + nrm[1], z + nrm[2])) continue;
                    GenFace(block, static_cast<Face>(f));
                    AddIndices(1);
                }
            }
}

std::array<float, SIZE * SIZE> Chunk::GenTerrain() {
    std::array<float, SIZE * SIZE> heightMap{};

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.SetFrequency(0.05f);
    int index = 0;

    for (int x = 0; x < SIZE; x++) {
        for (int z = 0; z < SIZE; z++) {
            heightMap[index++] = noise.GetNoise(static_cast<float>(x), static_cast<float>(z));
        }
    }

    return heightMap;
}

void Chunk::GenFace(const Block block , const Face face) {
    auto [vertices, uv] = block.GetFaceData(face);
    m_Vertices.append_range(vertices);
    m_UVs.append_range(uv);
}

void Chunk::AddIndices(const int numFaces) {
    for (int i = 0; i < numFaces; i++) {
        m_Indices.push_back(0 + m_indexCount);
        m_Indices.push_back(1 + m_indexCount);
        m_Indices.push_back(2 + m_indexCount);
        m_Indices.push_back(2 + m_indexCount);
        m_Indices.push_back(3 + m_indexCount);
        m_Indices.push_back(0 + m_indexCount);

        m_indexCount += 4;
    }
}

void Chunk::RebuildMesh(nvrhi::IDevice* device, nvrhi::ICommandList* cmd) {
    std::vector<Vertex> vertices;
    vertices.reserve(m_Vertices.size());
    for (size_t i = 0; i < m_Vertices.size(); ++i)
        vertices.push_back(Vertex{ m_Vertices[i], m_UVs[i] });

    if (m_Indices.empty()) { m_Mesh = Mesh(); return; }
    m_Mesh = Mesh::CreateMesh(device, cmd,
        vertices.data(),      static_cast<uint32_t>(vertices.size() * sizeof(Vertex)),
        m_Indices.data(),  static_cast<uint32_t>(m_Indices.size() * sizeof(uint32_t)),
        static_cast<uint32_t>(m_Indices.size()));
}

void Chunk::Draw(nvrhi::ICommandList* cmd, nvrhi::GraphicsState& state) const {
    if (!m_Mesh.IsValid()) return;
    m_Mesh.BindTo(state);
    cmd->setGraphicsState(state);
    cmd->drawIndexed(nvrhi::DrawArguments().setVertexCount(m_Mesh.IndexCount()));
}
