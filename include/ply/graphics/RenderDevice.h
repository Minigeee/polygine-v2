#pragma once

#include <ply/graphics/Pipeline.h>
#include <ply/graphics/Shader.h>
#include <ply/math/Types.h>

namespace ply {

class Window;
class GpuResource;

namespace priv {
    struct DeviceImpl;
}

///////////////////////////////////////////////////////////
/// \brief Class for interfacing with gpu
///
///////////////////////////////////////////////////////////
class RenderDevice {
    friend GpuResource;

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
    /// \brief TEMP : Bind current back buffer as render targets
    ///
    /////////////////////////////////////////////////////////////
    void bindBackBuffer();
    
    /////////////////////////////////////////////////////////////
    /// \brief Clear current render target
    ///
    /// \param color The color to clear the render target with
    ///
    /////////////////////////////////////////////////////////////
    void clear(const Vector3f& color);

    /////////////////////////////////////////////////////////////
    /// \brief Clear depth buffer
    ///
    /// \param depth The depth value to clear the depth buffer with
    ///
    /////////////////////////////////////////////////////////////
    void clearDepth(float depth);

    /////////////////////////////////////////////////////////////
    /// \brief Clear stencil buffer
    ///
    /// \param stencil The stencil value to clear the stencil buffer with
    ///
    /////////////////////////////////////////////////////////////
    void clearStencil(uint8_t stencil);

    /////////////////////////////////////////////////////////////
    /// \brief Draw to current render target using given pipeline
    ///
    /// \param pipeline The pipeline to use for rendering
    /// \param numVertices Number of vertices to draw
    ///
    /////////////////////////////////////////////////////////////
    void draw(Pipeline* pipeline, uint32_t numVertices);
    
    /////////////////////////////////////////////////////////////
    /// \brief Present the current back buffer
    ///
    /// \param sync Sync interval
    ///
    /////////////////////////////////////////////////////////////
    void present(uint32_t sync = 1);
    
private:
    priv::DeviceImpl* m_device;
};

} // namespace ply