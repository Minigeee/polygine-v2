#pragma once

#include <ply/graphics/GpuResource.h>

namespace ply {

namespace priv {
    struct RenderPassDesc;
}

///////////////////////////////////////////////////////////
/// \brief Class for managing the render pass API
///
/// This makes multi-pass rendering more efficient on tiling GPUs
///
///////////////////////////////////////////////////////////
class RenderPass : public GpuResource {
public:
    GPU_RESOURCE(RenderPass);

    ~RenderPass();
};

///////////////////////////////////////////////////////////
/// \brief Builder class for render pass gpu object
///
///////////////////////////////////////////////////////////
class RenderPassBuilder : public GpuResourceBuilder {
public:
    ///////////////////////////////////////////////////////////
    /// \brief Construct a render pass builder for a render device.
    /// \param device The render device pointer.
    ///
    ///////////////////////////////////////////////////////////
    RenderPassBuilder(RenderDevice* device);

    ///////////////////////////////////////////////////////////
    /// \brief Construct a render pass builder for a device implementation.
    /// \param device The device implementation pointer.
    ///
    ///////////////////////////////////////////////////////////
    RenderPassBuilder(priv::DeviceImpl* device);

    ///////////////////////////////////////////////////////////
    /// \brief Destructor. Cleans up builder resources.
    ///
    ///////////////////////////////////////////////////////////
    ~RenderPassBuilder();

    RenderPassBuilder(const RenderPassBuilder&) = delete;
    RenderPassBuilder& operator=(const RenderPassBuilder&) = delete;
    RenderPassBuilder(RenderPassBuilder&&) noexcept;
    RenderPassBuilder& operator=(RenderPassBuilder&&) noexcept;

private:
    priv::RenderPassDesc* m_desc; //!< Render pass descriptor
};

} // namespace ply