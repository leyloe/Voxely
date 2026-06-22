#ifndef VOXELY_APP_H
#define VOXELY_APP_H

#include <donut/app/ApplicationBase.h>
#include <donut/app/DeviceManager.h>
#include "Camera.h"
#include "Chunk.h"
#include "Texture.h"
#include "Pipeline.h"

using namespace donut;

class App : public app::IRenderPass {
    private:
        nvrhi::CommandListHandle m_CommandList;
        std::string m_WindowTitle;

        Pipeline m_Pipeline;
        Texture m_DirtTexture;
        Camera m_Camera;
        Chunk m_Chunk{ dm::float3(0.f, 0.f, 0.f) };



    public:
        using app::IRenderPass::IRenderPass;

        bool Init(std::string_view windowTitle, std::string_view shaderPath, const char *shaderFile);
        void BackBufferResizing() override;
        void Animate(float fElapsedTimeSeconds) override;
        void Render(nvrhi::IFramebuffer* framebuffer) override;

        bool KeyboardUpdate(int key, int scancode, int action, int mods) override;
        bool MousePosUpdate(double xpos, double ypos) override;
        bool MouseButtonUpdate(int button, int action, int mods) override;
};


#endif //VOXELY_APP_H
