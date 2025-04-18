#include <ply/graphics/Texture.h>

#include "RenderImpl.h"
#include <ply/core/PoolAllocator.h>
#include <ply/graphics/RenderDevice.h>

namespace ply {

namespace priv {

    ///////////////////////////////////////////////////////////
    TEXTURE_FORMAT getInternalFormat(uint32_t c, Type dtype) {
        if (dtype == Type::Uint8 || dtype == Type::Int8) {
            if (c == 1)
                return TEX_FORMAT_R8_UNORM;
            else if (c == 2)
                return TEX_FORMAT_RG8_UNORM;
            else if (c == 3 || c == 4)
                return TEX_FORMAT_RGBA8_UNORM;
        } else if (dtype == Type::Uint16 || dtype == Type::Int16) {
            if (c == 1)
                return TEX_FORMAT_R16_UNORM;
            else if (c == 2)
                return TEX_FORMAT_RG16_UNORM;
            else if (c == 3 || c == 4)
                return TEX_FORMAT_RGBA16_UNORM;
        } else if (dtype == Type::Float32) {
            if (c == 1)
                return TEX_FORMAT_R32_FLOAT;
            else if (c == 2)
                return TEX_FORMAT_RG32_FLOAT;
            else if (c == 3)
                return TEX_FORMAT_RGB32_FLOAT;
            else if (c == 4)
                return TEX_FORMAT_RGBA32_FLOAT;
        } else if (dtype == Type::Float16) {
            if (c == 1)
                return TEX_FORMAT_R16_FLOAT;
            else if (c == 2)
                return TEX_FORMAT_RG16_FLOAT;
            else if (c == 3 || c == 4)
                return TEX_FORMAT_RGBA16_FLOAT;
        }

        return TEX_FORMAT_UNKNOWN;
    }

} // namespace priv

///////////////////////////////////////////////////////////
Texture::~Texture() {
    if (m_device && m_resource) {
        m_device->m_textures.remove(m_handle);
    }
}

///////////////////////////////////////////////////////////
void Texture::update(
    const void* data,
    uint32_t stride,
    const Vector2u& pos,
    const Vector2u& size,
    uint32_t slice,
    uint32_t mip
) {
    CHECK_F(m_device != nullptr, "texture update requires a render device");

    Box UpdateBox;
    UpdateBox.MinX = pos.x;
    UpdateBox.MinY = pos.y;
    UpdateBox.MaxX = UpdateBox.MinX + size.x;
    UpdateBox.MaxY = UpdateBox.MinY + size.y;

    TextureSubResData SubresData;
    SubresData.Stride = stride;
    SubresData.pData = data;
    m_device->m_deviceContext->UpdateTexture(
        TEXTURE(m_resource),
        mip,
        slice,
        UpdateBox,
        SubresData,
        m_device->m_transitionMode,
        m_device->m_transitionMode
    );
}

///////////////////////////////////////////////////////////
void Texture::update(
    const Image& image,
    const Vector2u& pos,
    uint32_t slice,
    uint32_t mip
) {
    // Calculate stride
    uint32_t typeSize = 1;
    Type dtype = image.getDataType();
    if (dtype == Type::Uint16)
        typeSize = 2;
    else if (dtype == Type::Float32)
        typeSize = 4;
    uint32_t stride = image.getWidth() * image.getNumChannels() * typeSize;

    update(
        image.getData(),
        stride,
        pos,
        {image.getWidth(), image.getHeight()},
        slice,
        mip
    );
}

///////////////////////////////////////////////////////////
TextureFormat Texture::getFormat() const {
    const auto& desc = TEXTURE(m_resource)->GetDesc();
    return priv::convertTextureFormat(desc.Format);
}

///////////////////////////////////////////////////////////
TextureType Texture::getType() const {
    const auto& desc = TEXTURE(m_resource)->GetDesc();

    switch (desc.Type) {
    case RESOURCE_DIM_TEX_1D:
        return TextureType::Tex1D;
    case RESOURCE_DIM_TEX_2D:
        return TextureType::Tex2D;
    case RESOURCE_DIM_TEX_3D:
        return TextureType::Tex3D;
    case RESOURCE_DIM_TEX_CUBE:
        return TextureType::TexCube;
    case RESOURCE_DIM_TEX_1D_ARRAY:
        return TextureType::Tex1DArray;
    case RESOURCE_DIM_TEX_2D_ARRAY:
        return TextureType::Tex2DArray;
    case RESOURCE_DIM_TEX_CUBE_ARRAY:
        return TextureType::TexCubeArray;
    default:
        return TextureType::Undefined;
    }
}

///////////////////////////////////////////////////////////
Vector3u Texture::getSize() const {
    const auto& desc = TEXTURE(m_resource)->GetDesc();

    Vector3u size;
    size.x = desc.Width;
    size.y = desc.Height;
    size.z = desc.Depth;

    return size;
}

///////////////////////////////////////////////////////////
uint32_t Texture::getMips() const {
    const auto& desc = TEXTURE(m_resource)->GetDesc();
    return desc.MipLevels;
}

///////////////////////////////////////////////////////////
TextureBuilder::TextureBuilder(RenderDevice* device) :
    TextureBuilder(device->m_device) {}

///////////////////////////////////////////////////////////
TextureBuilder::TextureBuilder(priv::DeviceImpl* device) :
    GpuResourceBuilder(device) {
    m_desc = Pool<priv::TextureDesc>::alloc();

    m_desc->Type = RESOURCE_DIM_TEX_2D;
    m_desc->Format = TEX_FORMAT_RGBA8_UINT;

    m_desc->BindFlags = BIND_SHADER_RESOURCE;
    m_desc->Usage = USAGE_IMMUTABLE;
    m_desc->CPUAccessFlags = CPU_ACCESS_NONE;
}

///////////////////////////////////////////////////////////
TextureBuilder::~TextureBuilder() {
    if (m_desc) {
        Pool<priv::TextureDesc>::free(m_desc);
    }
}

///////////////////////////////////////////////////////////
TextureBuilder::TextureBuilder(TextureBuilder&& other) noexcept :
    m_desc(std::exchange(other.m_desc, nullptr)) {}

///////////////////////////////////////////////////////////
TextureBuilder& TextureBuilder::operator=(TextureBuilder&& other) noexcept {
    if (this != &other) {
        m_desc = std::exchange(other.m_desc, nullptr);
    }
    return *this;
}

///////////////////////////////////////////////////////////
TextureBuilder& TextureBuilder::bind(ResourceBind bind) {
    m_desc->BindFlags = static_cast<BIND_FLAGS>(bind);
    return *this;
}

///////////////////////////////////////////////////////////
TextureBuilder& TextureBuilder::usage(ResourceUsage usage) {
    m_desc->Usage = static_cast<USAGE>(usage);
    return *this;
}

///////////////////////////////////////////////////////////
TextureBuilder& TextureBuilder::access(ResourceAccess access) {
    m_desc->CPUAccessFlags = static_cast<CPU_ACCESS_FLAGS>(access);
    return *this;
}

///////////////////////////////////////////////////////////
TextureBuilder& TextureBuilder::type(TextureType type) {
    m_desc->Type = static_cast<RESOURCE_DIMENSION>(type);
    return *this;
}

///////////////////////////////////////////////////////////
TextureBuilder& TextureBuilder::format(TextureFormat format) {
    m_desc->Format = priv::convertTextureFormat(format);
    return *this;
}

///////////////////////////////////////////////////////////
TextureBuilder& TextureBuilder::mips(uint32_t mips) {
    m_desc->MipLevels = mips;
    if (mips != 1) {
        m_desc->MiscFlags |= MISC_TEXTURE_FLAG_GENERATE_MIPS;
    }
    return *this;
}

///////////////////////////////////////////////////////////
TextureBuilder& TextureBuilder::size(size_t w, size_t h, size_t d) {
    m_desc->Width = w;
    m_desc->Height = h;
    m_desc->Depth = d;
    return *this;
}

///////////////////////////////////////////////////////////
TextureBuilder& TextureBuilder::data(const void* data, uint32_t stride) {
    TextureSubResData subresData;
    subresData.pData = data;
    subresData.Stride = stride;
    m_desc->Data.push_back(subresData);

    // Track texture array size
    if (m_desc->Data.size() > 0) {
        m_desc->ArraySize = m_desc->Data.size();
    }

    return *this;
}

///////////////////////////////////////////////////////////
TextureBuilder& TextureBuilder::from(const Image& image) {
    Type dtype = image.getDataType();
    m_desc->Type = RESOURCE_DIM_TEX_2D;

    // Size
    m_desc->Width = image.getWidth();
    m_desc->Height = image.getHeight();

    // Get format
    auto format = priv::getInternalFormat(image.getNumChannels(), dtype);
    m_desc->Format = format;

    // Calculate stride
    uint32_t typeSize = 1;
    if (dtype == Type::Uint16)
        typeSize = 2;
    else if (dtype == Type::Float32)
        typeSize = 4;
    uint32_t stride = image.getWidth() * image.getNumChannels() * typeSize;

    return this->data(image.getData(), stride);
}

///////////////////////////////////////////////////////////
void TextureBuilder::setUpData() {
    uint32_t numTextures = m_desc->Data.size();
    CHECK_F(numTextures > 0, "no data provided");

    // Check if texture type needs to be changed to array
    if (m_desc->ArraySize > 1) {
        auto type = m_desc->Type;
        if (type == RESOURCE_DIM_TEX_2D)
            m_desc->Type = RESOURCE_DIM_TEX_2D_ARRAY;
        else if (type == RESOURCE_DIM_TEX_CUBE)
            m_desc->Type = RESOURCE_DIM_TEX_CUBE_ARRAY;
        else if (type == RESOURCE_DIM_TEX_1D)
            m_desc->Type = RESOURCE_DIM_TEX_1D_ARRAY;
    }

    // Determine how many levels
    uint32_t mips = m_desc->MipLevels;
    if (mips == 0)
        mips =
            std::round(std::log2(std::min(m_desc->Width, m_desc->Height))) + 1;

    if (mips == 1)
        return;

    std::vector<TextureSubResData> newData;
    newData.reserve(numTextures * mips);

    // Calculate space needed
    for (uint32_t t = 0; t < numTextures; ++t) {
        newData.push_back(m_desc->Data[t]);

        for (uint32_t lvl = 1; lvl < mips; ++lvl) {
            TextureSubResData data;
            data.pData = m_desc->Data[t].pData;
            newData.push_back(data);
        }
    }

    m_desc->Data = std::move(newData);
}

///////////////////////////////////////////////////////////
Texture TextureBuilder::create() {
    // Set up data for mip map generation
    if (m_desc->Data.size() > 0)
        setUpData();

    // Create data
    TextureData data;
    data.NumSubresources = m_desc->Data.size();
    data.pSubResources = m_desc->Data.data();
    data.pContext = m_device->m_deviceContext;

    // Create buffer
    RefCntAutoPtr<ITexture> texture;
    m_device->m_renderDevice->CreateTexture(
        *m_desc,
        data.NumSubresources > 0 ? &data : nullptr,
        &texture
    );

    // Generate mip maps
    if (m_desc->MipLevels != 1) {
        m_device->m_deviceContext->GenerateMips(
            texture->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE)
        );
    }

    // Register buffer
    Handle handle = m_device->m_textures.push(texture);

    return Texture(m_device, texture, handle);
}

} // namespace ply