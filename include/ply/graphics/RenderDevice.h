#pragma once

#include <ply/core/Macros.h>
#include <ply/graphics/Buffer.h>
#include <ply/graphics/Pipeline.h>
#include <ply/graphics/Texture.h>
#include <ply/graphics/Shader.h>
#include <ply/math/Types.h>
#include <ply/graphics/Image.h>

namespace ply {

class Window;
class GpuResource;
class RenderDevice;

namespace priv {
    struct DeviceImpl;
}

///////////////////////////////////////////////////////////
/// \brief Clear flags
///
///////////////////////////////////////////////////////////
enum class ClearFlag : uint8_t {
    Color = 1 << 0,
    Depth = 1 << 1,
    Stencil = 1 << 2
};
BIT_OPERATOR(ClearFlag);

///////////////////////////////////////////////////////////
/// \brief Class for interfacing with render device context
///
///////////////////////////////////////////////////////////
class RenderContext {
    friend RenderDevice;

public:
    RenderContext();
    RenderContext(const RenderContext&) = delete;
    RenderContext& operator=(const RenderContext&) = delete;
    RenderContext(RenderContext&&) noexcept = default;
    RenderContext& operator=(RenderContext&&) noexcept = default;

    /////////////////////////////////////////////////////////////
    /// \brief Clear render target
    ///
    /// \param flags The clear flags
    ///
    /////////////////////////////////////////////////////////////
    void clear(ClearFlag flags);

    /////////////////////////////////////////////////////////////
    /// \brief Set clear color
    ///
    /// \param color The color to clear the render target with
    ///
    /////////////////////////////////////////////////////////////
    void setClearColor(const Vector4f& color);

    /////////////////////////////////////////////////////////////
    /// \brief Set clear depth
    ///
    /// \param depth The depth value to clear the depth buffer with
    ///
    /////////////////////////////////////////////////////////////
    void setClearDepth(float depth);

    /////////////////////////////////////////////////////////////
    /// \brief Set clear stencil
    ///
    /// \param stencil The stencil value to clear the stencil buffer with
    ///
    /////////////////////////////////////////////////////////////
    void setClearStencil(uint8_t stencil);

    /////////////////////////////////////////////////////////////
    /// \brief Set current vertex buffers
    ///
    /// \param buffers The list of vertex buffers to set
    /// \param slot The start slot to set the vertex buffers to. The first
    /// vertex buffer is bound to the start slot, and each subsequent buffer is
    /// bound to the next slot.
    /// \param offsets The offsets to set for each buffer
    ///
    /////////////////////////////////////////////////////////////
    void setVertexBuffers(
        const BufferList& buffers,
        uint32_t slot = 0,
        const uint64_t* offsets = nullptr
    );

    /////////////////////////////////////////////////////////////
    /// \brief Set current index buffer
    ///
    /// \param buffer The index buffer to set
    /// \param offset The offset to start reading from the index buffer
    ///
    /////////////////////////////////////////////////////////////
    void setIndexBuffer(const Buffer& buffer, uint64_t offset = 0);

    /////////////////////////////////////////////////////////////
    /// \brief Draw to current render target using given pipeline
    ///
    /// \param pipeline The pipeline to use for rendering
    /// \param numVertices Number of vertices to draw
    /// \param binding The resource binding to use for rendering
    ///
    /////////////////////////////////////////////////////////////
    void draw(
        Pipeline& pipeline,
        uint32_t numVertices,
        ResourceBinding* binding = nullptr
    );

    /////////////////////////////////////////////////////////////
    /// \brief Draw to current render target using given pipeline
    ///
    /// \param pipeline The pipeline to use for rendering
    /// \param numVertices Number of vertices to draw
    /// \param binding The resource binding to use for rendering
    /// \param dtype The data type of the index buffer
    ///
    /////////////////////////////////////////////////////////////
    void drawIndexed(
        Pipeline& pipeline,
        uint32_t numVertices,
        ResourceBinding* binding = nullptr,
        Type dtype = Type::Uint32
    );

    /////////////////////////////////////////////////////////////
    /// \brief Present the current back buffer
    ///
    /// \param sync Sync interval
    ///
    /////////////////////////////////////////////////////////////
    void present(uint32_t sync = 1);

    /////////////////////////////////////////////////////////////
    /// \brief TEMP : Bind current back buffer as render targets
    ///
    /////////////////////////////////////////////////////////////
    void bindBackBuffer();

private:
    priv::DeviceImpl* m_device; //!< Pointer to device
    Vector4f m_clearColor;      //!< Clear color
    float m_clearDepth;         //!< Clear depth
    uint8_t m_clearStencil;     //!< Clear stencil
};

///////////////////////////////////////////////////////////
/// \brief Class for interfacing with gpu
///
///////////////////////////////////////////////////////////
class RenderDevice {
    friend GpuResource;
    friend GpuResourceBuilder;
    friend ShaderBuilder;
    friend BufferBuilder;
    friend TextureBuilder;

public:
    /////////////////////////////////////////////////////////////
    /// \brief Default constructor.
    ///
    /////////////////////////////////////////////////////////////
    RenderDevice();

    /////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    /////////////////////////////////////////////////////////////
    ~RenderDevice();

    RenderDevice(const RenderDevice&) = delete;
    RenderDevice& operator=(const RenderDevice&) = delete;
    RenderDevice(RenderDevice&&) noexcept;
    RenderDevice& operator=(RenderDevice&&) noexcept;

    /////////////////////////////////////////////////////////////
    /// \brief Initialize the render device.
    ///
    /// \param window The window to render to.
    /// \return True if initialization was successful.
    ///
    /////////////////////////////////////////////////////////////
    bool initialize(Window* window);

    /////////////////////////////////////////////////////////////
    /// \brief Set shader path
    ///
    /// Should be a semicolon separated list of paths to shader directories.
    /// The default is "shaders;shaders/inc".
    ///
    /// \param path Path to the shader directory
    ///
    /////////////////////////////////////////////////////////////
    void setShaderPath(const std::string& path);

    /////////////////////////////////////////////////////////////
    /// \brief Create a shader
    ///
    /// \return A shader builder object
    ///
    /////////////////////////////////////////////////////////////
    ShaderBuilder shader();

    /////////////////////////////////////////////////////////////
    /// \brief Create a pipeline
    ///
    /// \return A pipeline builder object
    ///
    /////////////////////////////////////////////////////////////
    PipelineBuilder pipeline();

    /////////////////////////////////////////////////////////////
    /// \brief Create a buffer
    ///
    /// \return A buffer builder object
    ///
    /////////////////////////////////////////////////////////////
    BufferBuilder buffer();

    /////////////////////////////////////////////////////////////
    /// \brief Create a texture
    ///
    /// \return A texture builder object
    ///
    /////////////////////////////////////////////////////////////
    TextureBuilder texture();

    /////////////////////////////////////////////////////////////
    /// \brief Create a texture from an image
    ///
    /// \param image The image to create the texture from
    /// \return A texture
    ///
    /////////////////////////////////////////////////////////////
    Texture texture(const Image& image);

    RenderContext context; //!< Render context

private:
    priv::DeviceImpl* m_device;
};

} // namespace ply