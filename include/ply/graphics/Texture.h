#pragma once

#include <ply/graphics/GpuResource.h>
#include <ply/graphics/Image.h>
#include <ply/graphics/Types.h>

namespace ply {

namespace priv {
    struct TextureDesc;
}

///////////////////////////////////////////////////////////
/// \brief A GPU texture resource
///
///////////////////////////////////////////////////////////
class Texture : public GpuResource {
public:
    GPU_RESOURCE(Texture);

    ///////////////////////////////////////////////////////////
    /// \brief Destructor. Releases the GPU texture resource.
    ///
    ///////////////////////////////////////////////////////////
    ~Texture();

    ///////////////////////////////////////////////////////////
    /// \brief Update a region of the texture with raw data.
    /// \param data Pointer to the data to upload.
    /// \param stride Row stride in bytes.
    /// \param pos Offset in the texture (x, y).
    /// \param size Size of the region to update (width, height).
    /// \param slice Array slice to update (default 0).
    /// \param mip Mipmap level to update (default 0).
    ///
    ///////////////////////////////////////////////////////////
    void update(
        const void* data,
        uint32_t stride,
        const Vector2u& pos,
        const Vector2u& size,
        uint32_t slice = 0,
        uint32_t mip = 0
    );

    ///////////////////////////////////////////////////////////
    /// \brief Update a region of the texture from an Image object.
    /// \param image The image to upload.
    /// \param pos Offset in the texture (x, y).
    /// \param slice Array slice to update (default 0).
    /// \param mip Mipmap level to update (default 0).
    ///
    ///////////////////////////////////////////////////////////
    void update(
        const Image& image,
        const Vector2u& pos,
        uint32_t slice = 0,
        uint32_t mip = 0
    );

    ///////////////////////////////////////////////////////////
    /// \brief Get the type of the texture (e.g., 2D, 3D, Cube).
    /// \return Texture type.
    ///
    ///////////////////////////////////////////////////////////
    TextureType getType() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the format of the texture.
    /// \return Texture format.
    ///
    ///////////////////////////////////////////////////////////
    TextureFormat getFormat() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the size of the texture (width, height, depth).
    /// \return Texture size as a Vector3u.
    ///
    ///////////////////////////////////////////////////////////
    Vector3u getSize() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the number of mipmap levels.
    /// \return Number of mipmap levels.
    ///
    ///////////////////////////////////////////////////////////
    uint32_t getMips() const;
};

///////////////////////////////////////////////////////////
/// \brief Texture builder
///
///////////////////////////////////////////////////////////
class TextureBuilder : public GpuResourceBuilder {
public:
    ///////////////////////////////////////////////////////////
    /// \brief Construct a texture builder for a render device.
    /// \param device The render device pointer.
    ///
    ///////////////////////////////////////////////////////////
    TextureBuilder(RenderDevice* device);

    ///////////////////////////////////////////////////////////
    /// \brief Construct a texture builder for a device implementation.
    /// \param device The device implementation pointer.
    ///
    ///////////////////////////////////////////////////////////
    TextureBuilder(priv::DeviceImpl* device);

    ///////////////////////////////////////////////////////////
    /// \brief Destructor. Cleans up builder resources.
    ///
    ///////////////////////////////////////////////////////////
    ~TextureBuilder();

    TextureBuilder(const TextureBuilder&) = delete;
    TextureBuilder& operator=(const TextureBuilder&) = delete;
    TextureBuilder(TextureBuilder&&) noexcept;
    TextureBuilder& operator=(TextureBuilder&&) noexcept;

    ///////////////////////////////////////////////////////////
    /// \brief Set the resource bind flags for the texture.
    /// \param bind Resource bind flags (e.g., ShaderResource, RenderTarget).
    ///
    ///////////////////////////////////////////////////////////
    TextureBuilder& bind(ResourceBind bind);

    ///////////////////////////////////////////////////////////
    /// \brief Set the usage type for the texture (e.g., Immutable, Dynamic).
    /// \param usage Resource usage type.
    ///
    ///////////////////////////////////////////////////////////
    TextureBuilder& usage(ResourceUsage usage);

    ///////////////////////////////////////////////////////////
    /// \brief Set the CPU access flags for the texture.
    /// \param access CPU access flags (e.g., Read, Write).
    ///
    ///////////////////////////////////////////////////////////
    TextureBuilder& access(ResourceAccess access);

    ///////////////////////////////////////////////////////////
    /// \brief Set the texture type (e.g., 2D, 3D, Cube).
    /// \param type Texture type.
    ///
    ///////////////////////////////////////////////////////////
    TextureBuilder& type(TextureType type);

    ///////////////////////////////////////////////////////////
    /// \brief Set the texture format.
    /// \param format Texture format.
    ///
    ///////////////////////////////////////////////////////////
    TextureBuilder& format(TextureFormat format);

    ///////////////////////////////////////////////////////////
    /// \brief Set the size of the texture.
    /// \param width Width in pixels.
    /// \param height Height in pixels.
    /// \param depth Depth in pixels (default 1).
    ///
    ///////////////////////////////////////////////////////////
    TextureBuilder& size(size_t width, size_t height, size_t depth = 1);

    ///////////////////////////////////////////////////////////
    /// \brief Set the number of mipmap levels.
    /// \param mips Number of mipmap levels.
    ///
    ///////////////////////////////////////////////////////////
    TextureBuilder& mips(uint32_t mips);

    ///////////////////////////////////////////////////////////
    /// \brief Set the initial data for the texture from raw memory.
    /// \param data Pointer to the data.
    /// \param stride Row stride in bytes (default 0).
    ///
    /// \remark This function can be called multiple times to create a texture array
    ///
    ///////////////////////////////////////////////////////////
    TextureBuilder& data(const void* data, uint32_t stride = 0);

    ///////////////////////////////////////////////////////////
    /// \brief Set the initial data for the texture from an Image object.
    /// \param image The image to upload.
    ///
    /// \remark This function can be called multiple times to create a texture array
    ///
    ///////////////////////////////////////////////////////////
    TextureBuilder& from(const Image& image);

    ///////////////////////////////////////////////////////////
    /// \brief Create the texture resource with the specified parameters.
    /// \return The created Texture object.
    ///
    ///////////////////////////////////////////////////////////
    Texture create();

private:
    ///////////////////////////////////////////////////////////
    /// \brief Internal helper to set up subresource data for mipmaps/arrays.
    ///
    ///////////////////////////////////////////////////////////
    void setUpData();

private:
    priv::TextureDesc* m_desc; //!< Texture descriptor
};

} // namespace ply