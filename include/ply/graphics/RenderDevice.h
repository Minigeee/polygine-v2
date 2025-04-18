#pragma once

#include <ply/core/Macros.h>
#include <ply/graphics/Buffer.h>
#include <ply/graphics/Framebuffer.h>
#include <ply/graphics/Image.h>
#include <ply/graphics/Pipeline.h>
#include <ply/graphics/Shader.h>
#include <ply/graphics/Texture.h>
#include <ply/math/Types.h>

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
    ///////////////////////////////////////////////////////////
    void setIndexBuffer(const Buffer& buffer, uint64_t offset = 0);

    ///////////////////////////////////////////////////////////
    /// \brief Set current pipeline
    ///
    /// \param pipeline The pipeline to set
    ///
    ///////////////////////////////////////////////////////////
    void setPipeline(const Pipeline& pipeline);

    ///////////////////////////////////////////////////////////
    /// \brief Set current resource binding to use for current pipeline
    ///
    /// \param binding The resource binding to set
    ///
    ///////////////////////////////////////////////////////////
    void setResourceBinding(const ResourceBinding& binding);

    ///////////////////////////////////////////////////////////
    /// \brief Set current render target
    ///
    /// \param framebuffer The framebuffer to set as the current render target
    ///
    ///////////////////////////////////////////////////////////
    void setRenderTarget(Framebuffer& framebuffer);

    ///////////////////////////////////////////////////////////
    /// \brief Transitions all resources to the correct state
    ///
    /// \param binding The resource binding to set states for
    ///
    /// \remark This method is used to ensure that all resources in the
    /// resource binding are in the correct state before using the
    /// render pass API.
    ///
    ///////////////////////////////////////////////////////////
    void setResourceStates(ResourceBinding& binding);

    ///////////////////////////////////////////////////////////
    /// \brief Set render pass mode
    ///
    /// This enables render pass mode, which is needed to use the render pass
    /// API. This disables automatic resource state transitions and must be
    /// called before starting a render pass.
    ///
    /// \param enabled True to enable render pass mode, false to disable
    ///
    ///////////////////////////////////////////////////////////
    void setRenderPassMode(bool enabled);

    ///////////////////////////////////////////////////////////
    /// \brief Draw to current render target using current pipeline
    ///
    /// \param numVertices Number of vertices to draw
    /// \param instances Number of instances to draw
    ///
    /////////////////////////////////////////////////////////////
    void draw(uint32_t numVertices, uint32_t instances = 1);

    /////////////////////////////////////////////////////////////
    /// \brief Draw to current render target using current pipeline
    ///
    /// \param numVertices Number of vertices to draw
    /// \param instances Number of instances to draw
    /// \param dtype The data type of the index buffer
    ///
    /////////////////////////////////////////////////////////////
    void drawIndexed(
        uint32_t numVertices,
        uint32_t instances = 1,
        Type dtype = Type::Uint32
    );

    /////////////////////////////////////////////////////////////
    /// \brief Present the current back buffer
    ///
    /// \param sync Sync interval
    ///
    /////////////////////////////////////////////////////////////
    void present(uint32_t sync = 1);

private:
    priv::DeviceImpl* m_device; //!< Pointer to device
    Vector4f m_clearColor;      //!< Clear color
    float m_clearDepth;         //!< Clear depth
    uint8_t m_clearStencil;     //!< Clear stencil
    uint8_t m_transitionMode;   //!< Resource state transition mode

    Framebuffer* m_currentFramebuffer; //!< Current framebuffer
};

///////////////////////////////////////////////////////////
/// \brief Class for interfacing with gpu
///
/// The RenderDevice class is the main interface for interacting with the GPU.
/// It provides methods for creating and managing GPU resources such as buffers,
/// textures, shaders, pipelines, and framebuffers. RenderDevice also manages
/// the rendering context and is responsible for initializing the graphics
/// backend and connecting to a window.
///
/// Resource creation is performed using builder objects, which provide a fluent
/// interface for configuring and constructing resources. See the examples below
/// for typical usage.
///
/// Usage example (creating a vertex buffer):
/// \code
/// ply::Buffer vertexBuffer = device.buffer()
///     .bind(ply::ResourceBind::VertexBuffer)
///     .usage(ply::ResourceUsage::Immutable)
///     .size(sizeof(vertices))
///     .data(vertices, sizeof(vertices))
///     .create();
/// \endcode
///
/// Usage example (creating a texture from an image):
/// \code
/// ply::Texture texture = device.texture()
///     .from(image)
///     .mips(4)
///     .create();
/// \endcode
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
    /// Returns a ShaderBuilder for configuring and creating a shader.
    ///
    /// Usage example:
    /// \code
    /// ply::Shader shader = device.shader()
    ///     .type(ply::Shader::Vertex)
    ///     .file("shader.vsh")
    ///     .load();
    /// \endcode
    ///
    /// \return A shader builder object
    /////////////////////////////////////////////////////////////
    ShaderBuilder shader();

    /////////////////////////////////////////////////////////////
    /// \brief Create a pipeline
    ///
    /// Returns a PipelineBuilder for configuring and creating a pipeline.
    ///
    /// Usage example:
    /// \code
    /// ply::Pipeline pipeline = device.pipeline()
    ///     .shader(&vertexShader)
    ///     .shader(&pixelShader)
    ///     .addInputLayout(0, 0, 3, ply::Type::Float32)
    ///     .targetFormat(framebuffer)
    ///     .create();
    /// \endcode
    ///
    /// \return A pipeline builder object
    /////////////////////////////////////////////////////////////
    PipelineBuilder pipeline();

    /////////////////////////////////////////////////////////////
    /// \brief Create a buffer
    ///
    /// Returns a BufferBuilder for configuring and creating a buffer.
    ///
    /// Usage example:
    /// \code
    /// ply::Buffer buffer = device.buffer()
    ///     .bind(ply::ResourceBind::VertexBuffer)
    ///     .usage(ply::ResourceUsage::Dynamic)
    ///     .size(1024)
    ///     .create();
    /// \endcode
    ///
    /// \return A buffer builder object
    /////////////////////////////////////////////////////////////
    BufferBuilder buffer();

    /////////////////////////////////////////////////////////////
    /// \brief Create a texture
    ///
    /// Returns a TextureBuilder for configuring and creating a texture.
    ///
    /// Usage example:
    /// \code
    /// ply::Texture texture = device.texture()
    ///     .size(256, 256)
    ///     .format(ply::TextureFormat::Rgba8)
    ///     .mips(1)
    ///     .create();
    /// \endcode
    ///
    /// \return A texture builder object
    /////////////////////////////////////////////////////////////
    TextureBuilder texture();

    /////////////////////////////////////////////////////////////
    /// \brief Create a texture from an image
    ///
    /// Creates a texture directly from an Image object, optionally generating
    /// a specified number of mipmap levels.
    ///
    /// Usage example:
    /// \code
    /// ply::Texture texture = device.texture(image, 0); // 0 = full mip chain
    /// \endcode
    ///
    /// \param image The image to create the texture from
    /// \param mips Number of mipmap levels to generate (0 for full mip chain)
    /// \return A texture
    /////////////////////////////////////////////////////////////
    Texture texture(const Image& image, uint32_t mips = 1);

    /////////////////////////////////////////////////////////////
    /// \brief Create a framebuffer
    ///
    /// Returns a Framebuffer object for use as a render target.
    ///
    /// Usage example:
    /// \code
    /// ply::Framebuffer framebuffer = device.framebuffer();
    /// framebuffer.attachColor({width, height});
    /// framebuffer.attachDepth({width, height});
    /// \endcode
    ///
    /// \return A framebuffer object
    /////////////////////////////////////////////////////////////
    Framebuffer framebuffer();

    /////////////////////////////////////////////////////////////
    /// \brief Get render device implementation
    ///
    /////////////////////////////////////////////////////////////
    priv::DeviceImpl* getImpl() const;

    /////////////////////////////////////////////////////////////
    /// \brief Get required byte alignment offset for constant buffers
    ///
    /// \return Byte offset alignement
    ///
    /////////////////////////////////////////////////////////////
    uint32_t getConstantBufferAlignment() const;

    /////////////////////////////////////////////////////////////
    /// \brief Get required byte alignment offset for structured buffers
    ///
    /// \return Byte offset alignement
    ///
    /////////////////////////////////////////////////////////////
    uint32_t getStructuredBufferAlignment() const;

public:
    RenderContext context; //!< Render context

private:
    priv::DeviceImpl* m_device;
};

} // namespace ply