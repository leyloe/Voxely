#ifndef VOXELY_PIPELINE_H
#define VOXELY_PIPELINE_H

#include <nvrhi/nvrhi.h>
#include <vector>

namespace donut::engine { class ShaderFactory; }

class Pipeline {
public:
    Pipeline() = default;

    static Pipeline Create(nvrhi::IDevice* device,
                           donut::engine::ShaderFactory& shaderFactory,
                           const char* hlslFile,
                           const nvrhi::VertexAttributeDesc* attributes, uint32_t attributeCount,
                           uint32_t constantBufferBytes,
                           const std::vector<nvrhi::BindingSetItem>& resourceBindings);

    void SetConstants(nvrhi::ICommandList* commandList, const void* data, size_t size) const;

    void Apply(nvrhi::GraphicsState& state, const nvrhi::IFramebuffer* framebuffer);

    void Invalidate();

    [[nodiscard]] bool IsValid() const;

private:
    nvrhi::DeviceHandle m_Device;
    nvrhi::ShaderHandle m_VertexShader;
    nvrhi::ShaderHandle m_PixelShader;
    nvrhi::InputLayoutHandle m_InputLayout;
    nvrhi::BufferHandle m_ConstantBuffer;
    nvrhi::BindingLayoutHandle m_BindingLayout;
    nvrhi::BindingSetHandle m_BindingSet;
    nvrhi::GraphicsPipelineHandle m_Pso;
};

#endif //VOXELY_PIPELINE_H
