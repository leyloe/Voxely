#include <donut/engine/ShaderFactory.h>
#include <nvrhi/utils.h>

#include "App.h"
#include "Vertex.h"

bool App::Init(const std::string_view windowTitle, const std::string_view shaderPath, const char *shaderFile) {
    m_WindowTitle = windowTitle;

    const std::filesystem::path appShaderPath = app::GetDirectoryWithExecutable() / shaderPath /  app::GetShaderTypeName(GetDevice()->getGraphicsAPI());

    const auto nativeFS = std::make_shared<vfs::NativeFileSystem>();
    engine::ShaderFactory shaderFactory(GetDevice(), nativeFS, appShaderPath);

    m_Chunk.Generate();

    m_CommandList = GetDevice()->createCommandList();

    m_CommandList->open();
    m_Chunk.RebuildMesh(GetDevice(), m_CommandList);   // upload to the chunk's Mesh
    m_DirtTexture = Texture::LoadFromFile(GetDevice(), nativeFS, m_CommandList,
        app::GetDirectoryWithExecutable() / "textures" / "dirt.png");
    m_CommandList->close();
    GetDevice()->executeCommandList(m_CommandList);

    if (!m_DirtTexture.IsValid())
    {
        log::error("Failed to load dirt.png");
        return false;
    }

    const nvrhi::VertexAttributeDesc attributes[] = {
        nvrhi::VertexAttributeDesc().setName("POSITION").setFormat(nvrhi::Format::RGB32_FLOAT)
            .setOffset(offsetof(Vertex, position)).setElementStride(sizeof(Vertex)),
        nvrhi::VertexAttributeDesc().setName("TEXCOORD").setFormat(nvrhi::Format::RG32_FLOAT)
            .setOffset(offsetof(Vertex, uv)).setElementStride(sizeof(Vertex)),
    };

    m_Pipeline = Pipeline::Create(GetDevice(), shaderFactory, shaderFile,
        attributes, static_cast<uint32_t>(std::size(attributes)),
        sizeof(dm::float4x4),
        {
            nvrhi::BindingSetItem::Texture_SRV(0, m_DirtTexture.GetTexture()),
            nvrhi::BindingSetItem::Sampler(0, m_DirtTexture.GetSampler()),
        });

    if (!m_Pipeline.IsValid())
    {
        return false;
    }

    m_Camera.LookAt(dm::float3(8.f, 25.f, 40.f), dm::float3(8.f, 8.f, 8.f));
    m_Camera.SetMoveSpeed(15.f);
    m_Camera.SetPerspective(60.f, 0.1f, 1000.f);

    return true;
}

void App::BackBufferResizing()
{
    m_Pipeline.Invalidate();
}

void App::Animate(const float fElapsedTimeSeconds) {
    m_Camera.Animate(fElapsedTimeSeconds);
    GetDeviceManager()->SetInformativeWindowTitle(m_WindowTitle.c_str());
}

bool App::KeyboardUpdate(const int key, const int scancode, const int action, const int mods)  { m_Camera.KeyboardUpdate(key, scancode, action, mods); return true; }
bool App::MousePosUpdate(const double xpos, const double ypos)          { m_Camera.MousePosUpdate(xpos, ypos);       return true; }
bool App::MouseButtonUpdate(const int button, const int action, const int mods)      { m_Camera.MouseButtonUpdate(button, action, mods); return true; }

void App::Render(nvrhi::IFramebuffer* framebuffer)
{
    const auto& fb = framebuffer->getFramebufferInfo();
    const float aspect = static_cast<float>(fb.width) / static_cast<float>(fb.height);
    const dm::float4x4 modelToClip = m_Camera.GetViewProjMatrix(aspect);   // model = identity for now

    m_CommandList->open();
    nvrhi::utils::ClearColorAttachment(m_CommandList, framebuffer, 0, nvrhi::Color(0.1f, 0.1f, 0.15f, 1.f));
    nvrhi::utils::ClearDepthStencilAttachment(m_CommandList, framebuffer, 1.f, 0);
    m_Pipeline.SetConstants(m_CommandList, &modelToClip, sizeof(modelToClip));

    nvrhi::GraphicsState state;
    state.framebuffer = framebuffer;
    state.viewport.addViewportAndScissorRect(fb.getViewport());
    m_Pipeline.Apply(state, framebuffer);

    m_Chunk.Draw(m_CommandList, state);

    m_CommandList->close();
    GetDevice()->executeCommandList(m_CommandList);
}
