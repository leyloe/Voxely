#include "Mesh.h"
#include <utility>

Mesh::Mesh(nvrhi::BufferHandle vb, nvrhi::BufferHandle ib, const uint32_t indexCount, const nvrhi::Format ibFormat)
    : vertexBuffer(std::move(vb)), indexBuffer(std::move(ib)), numIndices(indexCount), indexFormat(ibFormat) {}

void Mesh::BindTo(nvrhi::GraphicsState& state) const {
    state.addVertexBuffer(nvrhi::VertexBufferBinding{ vertexBuffer.Get(), 0, 0 });
    state.indexBuffer = nvrhi::IndexBufferBinding{ indexBuffer.Get(), indexFormat, 0 };
}
uint32_t Mesh::IndexCount() const { return numIndices; }
bool     Mesh::IsValid()    const { return vertexBuffer && indexBuffer && numIndices > 0; }

Mesh Mesh::CreateMesh(nvrhi::IDevice* device, nvrhi::ICommandList* cmd,
                const void* vtx, const uint32_t vtxBytes,
                const void* idx, const uint32_t idxBytes, uint32_t indexCount) {
    auto vb = device->createBuffer(nvrhi::BufferDesc().setByteSize(vtxBytes).setIsVertexBuffer(true)
        .setInitialState(nvrhi::ResourceStates::VertexBuffer).setKeepInitialState(true).setDebugName("VB"));
    auto ib = device->createBuffer(nvrhi::BufferDesc().setByteSize(idxBytes).setIsIndexBuffer(true)
        .setInitialState(nvrhi::ResourceStates::IndexBuffer).setKeepInitialState(true).setDebugName("IB"));
    cmd->writeBuffer(vb, vtx, vtxBytes);   // must be between cmd->open()/close()
    cmd->writeBuffer(ib, idx, idxBytes);
    return {vb, ib, indexCount, nvrhi::Format::R32_UINT};
}