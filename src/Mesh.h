#ifndef VOXELY_MESH_H
#define VOXELY_MESH_H
#include "nvrhi/nvrhi.h"

class Mesh {
    nvrhi::BufferHandle vertexBuffer;
    nvrhi::BufferHandle indexBuffer;
    uint32_t            numIndices  = 0;
    nvrhi::Format       indexFormat = nvrhi::Format::R32_UINT;

public:
    Mesh() = default;
    Mesh(nvrhi::BufferHandle vb, nvrhi::BufferHandle ib, uint32_t indexCount,
         nvrhi::Format ibFormat = nvrhi::Format::R32_UINT);

    void BindTo(nvrhi::GraphicsState& state) const;

    [[nodiscard]] uint32_t IndexCount() const;
    [[nodiscard]] bool     IsValid()    const;

    static Mesh CreateMesh(nvrhi::IDevice* device, nvrhi::ICommandList* cmd,
                const void* vtx, uint32_t vtxBytes,
                const void* idx, uint32_t idxBytes, uint32_t indexCount);
};

#endif //VOXELY_MESH_H