#ifndef VOXELY_APP_H
#define VOXELY_APP_H

#include <donut/app/ApplicationBase.h>
#include <donut/app/DeviceManager.h>

using namespace donut;

class App : public app::IRenderPass {
    private:
        nvrhi::ShaderHandle m_VertexShader;
        nvrhi::ShaderHandle m_PixelShader;
        nvrhi::GraphicsPipelineHandle m_Pipeline;
        nvrhi::CommandListHandle m_CommandList;
        std::string m_WindowTitle;

    public:
        using app::IRenderPass::IRenderPass;

        bool Init(std::string_view windowTitle, std::string_view shaderPath, const char *shaderFile);
        void BackBufferResizing() override;
        void Animate(float fElapsedTimeSeconds) override;
        void Render(nvrhi::IFramebuffer* framebuffer) override;
};


#endif //VOXELY_APP_H
