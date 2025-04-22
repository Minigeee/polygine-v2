#pragma once

#include <ply/core/Macros.h>
#include <ply/graphics/GpuResource.h>

namespace ply {

///////////////////////////////////////////////////////////
/// \brief Class for managing the render pass API
///
/// This makes multi-pass rendering more efficient on tiling GPUs
/// Due to complexity, creation of render passes is not yet implemented.
/// It can only be used from built in renderer classes.
///
///////////////////////////////////////////////////////////
class RenderPass : public GpuResource {
public:
    ///////////////////////////////////////////////////////////
    /// \brief Render pass type
    ///
    /// Render systems can decide to skip rendering for certain
    /// render passes (i.e. skipping detail foliage for reflections).
    ///
    ///////////////////////////////////////////////////////////
    enum Type : uint8_t {
        Default = 1 << 0,    //!< A default render pass
        Shadow = 1 << 1,     //!< A shadow render pass
        Reflection = 1 << 2, //!< A reflection render pass
    };

public:
    RenderPass() :
        GpuResource() {}
        
    RenderPass(priv::DeviceImpl* device, void* resource, Handle handle) :
        GpuResource(device, resource, handle) {}
};

BIT_OPERATOR(RenderPass::Type);

} // namespace ply