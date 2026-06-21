#include <donut/app/ApplicationBase.h>

#include "App.h"

using namespace donut;

static auto g_WindowTitle = "Voxely";
static auto g_shaderPath = "shaders/voxely";
static auto g_shaderFile = "shaders.hlsl";

#ifdef WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int __argc, const char** __argv)
#endif
{
    const nvrhi::GraphicsAPI api = app::GetGraphicsAPIFromCommandLine(__argc, __argv);
    app::DeviceManager* deviceManager = app::DeviceManager::Create(api);

    app::DeviceCreationParameters deviceParams;
    deviceParams.depthBufferFormat = nvrhi::Format::D24S8;   // make Donut attach a depth buffer to the swap chain
#ifdef _DEBUG
    deviceParams.enableDebugRuntime = true;
    deviceParams.enableNvrhiValidationLayer = true;
#endif

    if (!deviceManager->CreateWindowDeviceAndSwapChain(deviceParams, g_WindowTitle))
    {
        log::fatal("Cannot initialize a graphics device with the requested parameters");
        return 1;
    }

    {
        App app(deviceManager);
        if (app.Init(g_WindowTitle, g_shaderPath, g_shaderFile))
        {
            deviceManager->AddRenderPassToBack(&app);
            deviceManager->RunMessageLoop();
            deviceManager->RemoveRenderPass(&app);
        }
    }

    deviceManager->Shutdown();

    delete deviceManager;

    return 0;
}