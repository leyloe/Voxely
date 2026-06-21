#include "Pipeline.h"

#include <donut/engine/ShaderFactory.h>
#include <nvrhi/utils.h>

using namespace donut;

Pipeline Pipeline::Create(nvrhi::IDevice* device,
                          engine::ShaderFactory& shaderFactory,
                          const char* hlslFile,
                          const nvrhi::VertexAttributeDesc* attributes, const uint32_t attributeCount,
                          const uint32_t constantBufferBytes,
                          const std::vector<nvrhi::BindingSetItem>& resourceBindings) {
    Pipeline p;
    p.m_Device = device;

    p.m_VertexShader = shaderFactory.CreateShader(hlslFile, "main_vs", nullptr, nvrhi::ShaderType::Vertex);
    p.m_PixelShader  = shaderFactory.CreateShader(hlslFile, "main_ps", nullptr, nvrhi::ShaderType::Pixel);
    if (!p.m_VertexShader || !p.m_PixelShader)
        return p;

    p.m_InputLayout = device->createInputLayout(attributes, attributeCount, p.m_VertexShader);

    p.m_ConstantBuffer = device->createBuffer(
        nvrhi::utils::CreateVolatileConstantBufferDesc(constantBufferBytes, "Constants", 16));

    nvrhi::BindingSetDesc setDesc;
    setDesc.bindings = { nvrhi::BindingSetItem::ConstantBuffer(0, p.m_ConstantBuffer) };
    for (const auto& item : resourceBindings)
        setDesc.bindings.push_back(item);

    nvrhi::utils::CreateBindingSetAndLayout(device, nvrhi::ShaderType::AllGraphics, 0,
        setDesc, p.m_BindingLayout, p.m_BindingSet);

    return p;
}

void Pipeline::SetConstants(nvrhi::ICommandList* commandList, const void* data, const size_t size) const {
    commandList->writeBuffer(m_ConstantBuffer, data, size);
}

void Pipeline::Apply(nvrhi::GraphicsState& state, const nvrhi::IFramebuffer* framebuffer) {
    if (!m_Pso) {
        nvrhi::GraphicsPipelineDesc desc;
        desc.VS = m_VertexShader;
        desc.PS = m_PixelShader;
        desc.inputLayout = m_InputLayout;
        desc.bindingLayouts = { m_BindingLayout };
        desc.primType = nvrhi::PrimitiveType::TriangleList;
        desc.renderState.depthStencilState.depthTestEnable = true;
        desc.renderState.depthStencilState.depthWriteEnable = true;
        desc.renderState.rasterState.cullMode = nvrhi::RasterCullMode::None;
        m_Pso = m_Device->createGraphicsPipeline(desc, framebuffer->getFramebufferInfo());
    }
    state.pipeline = m_Pso;
    state.bindings = { m_BindingSet };
}

void Pipeline::Invalidate() {
    m_Pso = nullptr;
}

bool Pipeline::IsValid() const {
    return m_VertexShader && m_PixelShader && m_BindingSet;
}
