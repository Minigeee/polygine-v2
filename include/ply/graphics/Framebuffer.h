#pragma once

#include <ply/graphics/Types.h>
#include <ply/math/Types.h>

namespace ply {

class RenderContext;
class RenderDevice;
class Texture;

namespace priv {
    class DeviceImpl;
    class FramebufferImpl;
} // namespace priv

///////////////////////////////////////////////////////////
/// \brief A class that acts as a render target with optional color, depth, and
/// stencil texture attachments
///
///////////////////////////////////////////////////////////
class Framebuffer {
    friend RenderContext;

public:
    ///////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ///////////////////////////////////////////////////////////
    Framebuffer();

    ///////////////////////////////////////////////////////////
    /// \brief Constructor
    ///
    ///////////////////////////////////////////////////////////
    Framebuffer(RenderDevice* device);

    ///////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ///////////////////////////////////////////////////////////
    ~Framebuffer();

    Framebuffer(const Framebuffer&) = delete;
    Framebuffer& operator=(const Framebuffer&) = delete;
    Framebuffer(Framebuffer&&) noexcept;
    Framebuffer& operator=(Framebuffer&&) noexcept;

    ///////////////////////////////////////////////////////////
    /// \brief Attach an existing color texture
    ///
    ///////////////////////////////////////////////////////////
    void attachColor(Texture* texture);

    ///////////////////////////////////////////////////////////
    /// \brief Create and attach a color texture
    ///
    /// \param size The size of the texture
    /// \param format The format of the texture
    /// \param bind The resource bind flags
    /// \return A pointer to the created texture
    ///
    ///////////////////////////////////////////////////////////
    Texture* attachColor(
        const Vector2u& size,
        TextureFormat format = TextureFormat::Rgba8,
        ResourceBind bind = ResourceBind::RenderTarget |
                            ResourceBind::ShaderResource
    );

    ///////////////////////////////////////////////////////////
    /// \brief Attach an existing depth texture
    ///
    ///////////////////////////////////////////////////////////
    void attachDepth(Texture* texture);

    ///////////////////////////////////////////////////////////
    /// \brief Create and attach a depth texture
    ///
    /// \param size The size of the texture
    /// \param format The format of the texture
    /// \param bind The resource bind flags
    /// \return A pointer to the created texture
    ///
    ///////////////////////////////////////////////////////////
    Texture* attachDepth(
        const Vector2u& size,
        TextureFormat format = TextureFormat::D32f,
        ResourceBind bind = ResourceBind::DepthStencil |
                            ResourceBind::ShaderResource
    );

    ///////////////////////////////////////////////////////////
    /// \brief Get the number of color texture attachments
    ///
    /// \return The number of color texture attachments
    ///
    ///////////////////////////////////////////////////////////
    size_t getNumColorTextures() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the color texture at the specified index
    ///
    /// \return A color texture
    ///
    ///////////////////////////////////////////////////////////
    Texture* getColorTexture(size_t index = 0) const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the depth texture at the specified index
    ///
    /// \return A depth texture
    ///
    ///////////////////////////////////////////////////////////
    Texture* getDepthTexture() const;

    ///////////////////////////////////////////////////////////
    /// \brief Bind the framebuffer to the render context
    ///
    ///////////////////////////////////////////////////////////
    void bind(RenderDevice* device);

    ///////////////////////////////////////////////////////////
    /// \brief Get framebuffer size in pixels
    ///
    /// \return The size of the framebuffer
    ///
    ///////////////////////////////////////////////////////////
    const Vector2u& getSize() const;

public:
    ///////////////////////////////////////////////////////////
    /// \brief Represents the default swapchain framebuffers. It does
    /// not hold any actual data, but can be used as a render target.
    ///
    ///////////////////////////////////////////////////////////
    static Framebuffer Default;

private:
    struct Attachment {
        Texture* m_texture = nullptr; //!< The texture attachment
        bool m_owner = false;         //!< Does framebuffer own texture
    };

    void bind(priv::DeviceImpl* device);

private:
    RenderDevice* m_device; //!< The render device implementation
    std::vector<Attachment> m_colorTextures; //!< The list of color textures
    std::vector<void*> m_colorTextureViews; //!< The list of color texture views
    Attachment m_depthTexture;              //!< The depth texture
    void* m_depthTextureView;               //!< The depth texture view
    Vector2u m_size;                        //!< The size of the framebuffer
};

} // namespace ply