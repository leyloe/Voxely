#include "Texture.h"

#include <donut/engine/TextureCache.h>

using namespace donut;

Texture Texture::LoadFromFile(nvrhi::IDevice* device,
                              const std::shared_ptr<vfs::IFileSystem>& fs,
                              nvrhi::ICommandList* commandList,
                              const std::filesystem::path& path,
                              const bool sRGB, const bool nearest) {
    Texture result;

    result.m_Sampler = device->createSampler(nvrhi::SamplerDesc()
        .setAllFilters(!nearest)
        .setAllAddressModes(nvrhi::SamplerAddressMode::Wrap));

    engine::TextureCache cache(device, fs, nullptr);
    const std::shared_ptr<engine::LoadedTexture> loaded =
        cache.LoadTextureFromFile(path, sRGB, nullptr, commandList);
    if (loaded)
        result.m_Texture = loaded->texture;

    return result;
}

nvrhi::ITexture* Texture::GetTexture() const { return m_Texture; }
nvrhi::ISampler* Texture::GetSampler() const { return m_Sampler; }
bool Texture::IsValid() const { return m_Texture != nullptr && m_Sampler != nullptr; }
