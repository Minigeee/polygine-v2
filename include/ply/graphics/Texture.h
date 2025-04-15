#pragma once

#include <ply/graphics/GpuResource.h>
#include <ply/graphics/Image.h>
#include <ply/graphics/Types.h>

namespace ply {

namespace priv {
    struct TextureDesc;
}

class Texture : public GpuResource {
public:
    GPU_RESOURCE(Texture);

    ~Texture();
};

///////////////////////////////////////////////////////////
/// \brief Texture builder
///
///////////////////////////////////////////////////////////
class TextureBuilder : public GpuResourceBuilder {
public:
    TextureBuilder(RenderDevice* device);
    TextureBuilder(priv::DeviceImpl* device);
    ~TextureBuilder();

    TextureBuilder(const TextureBuilder&) = delete;
    TextureBuilder& operator=(const TextureBuilder&) = delete;
    TextureBuilder(TextureBuilder&&) noexcept;
    TextureBuilder& operator=(TextureBuilder&&) noexcept;

    TextureBuilder& bind(ResourceBind bind);

    TextureBuilder& usage(ResourceUsage usage);

    TextureBuilder& access(ResourceAccess access);

    TextureBuilder& type(TextureType type);

    TextureBuilder& format(TextureFormat format);

    TextureBuilder& size(size_t width, size_t height, size_t depth = 1);

    TextureBuilder& mips(uint32_t mips);

    TextureBuilder& data(const void* data, uint32_t stride = 0);

    TextureBuilder& from(const Image& image);

    Texture create();

private:
    void setUpData();

private:
    priv::TextureDesc* m_desc;
};

} // namespace ply