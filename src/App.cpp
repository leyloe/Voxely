#include <donut/engine/ShaderFactory.h>
#include <nvrhi/utils.h>

#include "App.h"
#include "Vertex.h"

static const Vertex vertices[] = {
    // front face
    { .position = { -0.5f,  0.5f,  0.5f }, .uv = { 0.0f, 1.0f } }, //  0 top left
    { .position = {  0.5f,  0.5f,  0.5f }, .uv = { 1.0f, 1.0f } }, //  1 top right
    { .position = {  0.5f, -0.5f,  0.5f }, .uv = { 1.0f, 0.0f } }, //  2 bottom right
    { .position = { -0.5f, -0.5f,  0.5f }, .uv = { 0.0f, 0.0f } }, //  3 bottom left
    // right face
    { .position = {  0.5f,  0.5f,  0.5f }, .uv = { 0.0f, 1.0f } }, //  4
    { .position = {  0.5f,  0.5f, -0.5f }, .uv = { 1.0f, 1.0f } }, //  5
    { .position = {  0.5f, -0.5f, -0.5f }, .uv = { 1.0f, 0.0f } }, //  6
    { .position = {  0.5f, -0.5f,  0.5f }, .uv = { 0.0f, 0.0f } }, //  7
    // back face
    { .position = {  0.5f,  0.5f, -0.5f }, .uv = { 0.0f, 1.0f } }, //  8
    { .position = { -0.5f,  0.5f, -0.5f }, .uv = { 1.0f, 1.0f } }, //  9
    { .position = { -0.5f, -0.5f, -0.5f }, .uv = { 1.0f, 0.0f } }, // 10
    { .position = {  0.5f, -0.5f, -0.5f }, .uv = { 0.0f, 0.0f } }, // 11
    // left face
    { .position = { -0.5f,  0.5f, -0.5f }, .uv = { 0.0f, 1.0f } }, // 12
    { .position = { -0.5f,  0.5f,  0.5f }, .uv = { 1.0f, 1.0f } }, // 13
    { .position = { -0.5f, -0.5f,  0.5f }, .uv = { 1.0f, 0.0f } }, // 14
    { .position = { -0.5f, -0.5f, -0.5f }, .uv = { 0.0f, 0.0f } }, // 15
    // top face
    { .position = { -0.5f,  0.5f, -0.5f }, .uv = { 0.0f, 1.0f } }, // 16
    { .position = {  0.5f,  0.5f, -0.5f }, .uv = { 1.0f, 1.0f } }, // 17
    { .position = {  0.5f,  0.5f,  0.5f }, .uv = { 1.0f, 0.0f } }, // 18
    { .position = { -0.5f,  0.5f,  0.5f }, .uv = { 0.0f, 0.0f } }, // 19
    // bottom face
    { .position = { -0.5f, -0.5f,  0.5f }, .uv = { 0.0f, 1.0f } }, // 20
    { .position = {  0.5f, -0.5f,  0.5f }, .uv = { 1.0f, 1.0f } }, // 21
    { .position = {  0.5f, -0.5f, -0.5f }, .uv = { 1.0f, 0.0f } }, // 22
    { .position = { -0.5f, -0.5f, -0.5f }, .uv = { 0.0f, 0.0f } }, // 23
};

static const uint32_t indices[] = {
    0,  1,  2,   0,  2,  3,    // front
    4,  5,  6,   4,  6,  7,    // right
    8,  9,  10,  8,  10, 11,   // back
    12, 13, 14,  12, 14, 15,   // left
    16, 17, 18,  16, 18, 19,   // top
    20, 21, 22,  20, 22, 23,   // bottom
};

bool App::Init(const std::string_view windowTitle, const std::string_view shaderPath, const char *shaderFile) {
    m_WindowTitle = windowTitle;

    const std::filesystem::path appShaderPath = app::GetDirectoryWithExecutable() / shaderPath /  app::GetShaderTypeName(GetDevice()->getGraphicsAPI());

    const auto nativeFS = std::make_shared<vfs::NativeFileSystem>();
    engine::ShaderFactory shaderFactory(GetDevice(), nativeFS, appShaderPath);

    m_CommandList = GetDevice()->createCommandList();

    m_CommandList->open();
    m_CubeMesh = Mesh::CreateMesh(GetDevice(), m_CommandList,
        vertices, static_cast<uint32_t>(sizeof(vertices)),
        indices,  static_cast<uint32_t>(sizeof(indices)), static_cast<uint32_t>(std::size(indices)));

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

    m_Camera.LookAt(dm::float3(0.f, 1.f, 3.f), dm::float3(0.f, 0.f, 0.f));
    m_Camera.SetMoveSpeed(3.f);
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
    m_CubeMesh.BindTo(state);

    m_CommandList->setGraphicsState(state);
    m_CommandList->drawIndexed(nvrhi::DrawArguments().setVertexCount(m_CubeMesh.IndexCount()));
    m_CommandList->close();
    GetDevice()->executeCommandList(m_CommandList);
}
