#ifndef VOXELY_CHUNK_H
#define VOXELY_CHUNK_H
#include <vector>

#include "Block.h"
#include "Vertex.h"
#include "Mesh.h"
#include "donut/core/math/float.h"
#include "nvrhi/nvrhi.h"

#define SIZE 16
#define HEIGHT 32

class Chunk {
    std::vector<dm::float3> m_Vertices;
    std::vector<dm::float2> m_UVs;
    std::vector<uint32_t>   m_Indices;
    donut::math::float3 m_Position;
    unsigned int m_indexCount{};
    Mesh m_Mesh;
    std::array<BlockId, SIZE * HEIGHT * SIZE> m_ChunkBlocks{};
    BlockId m_BlockID;

public:
        explicit Chunk(donut::math::float3 pos, BlockId blockID = BlockId::Air);
        void Generate();
        void AddIndices(int numFaces);
        void RebuildMesh(nvrhi::IDevice* device, nvrhi::ICommandList* cmd);
        void Draw(nvrhi::ICommandList* cmd, nvrhi::GraphicsState& state) const;
        void GenFace(Block block ,Face face);
        static std::array<float, SIZE * SIZE> GenTerrain();

    private:
        static int  Index(int x, int y, int z) ;
        [[nodiscard]] bool IsSolidAt(int x, int y, int z) const;
        std::array<float, SIZE * SIZE> m_heightMap = GenTerrain();
};

#endif //VOXELY_CHUNK_H
