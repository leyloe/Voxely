#ifndef VOXELY_TEXTURE_H
#define VOXELY_TEXTURE_H

#include <nvrhi/nvrhi.h>
#include <filesystem>
#include <memory>

namespace donut::vfs { class IFileSystem; }

class Texture {
public:
    Texture() = default;

    static Texture LoadFromFile(nvrhi::IDevice* device,
                                const std::shared_ptr<donut::vfs::IFileSystem>& fs,
                                nvrhi::ICommandList* commandList,
                                const std::filesystem::path& path,
                                bool sRGB = true,
                                bool nearest = true);

    [[nodiscard]] nvrhi::ITexture* GetTexture() const;
    [[nodiscard]] nvrhi::ISampler* GetSampler() const;
    [[nodiscard]] bool IsValid() const;

private:
    nvrhi::TextureHandle m_Texture;
    nvrhi::SamplerHandle m_Sampler;
};

#endif //VOXELY_TEXTURE_H
