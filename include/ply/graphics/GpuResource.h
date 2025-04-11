#pragma once

#include <ply/core/Handle.h>

namespace ply {

class RenderDevice;
class Pipeline;
class ResourceBinding;

namespace priv {
    struct DeviceImpl;
}

class GpuResource {
public:
    GpuResource();
    GpuResource(RenderDevice* device);
    GpuResource(priv::DeviceImpl* device, void* resource, Handle handle);

    GpuResource(const GpuResource&) = delete;
    GpuResource& operator=(const GpuResource&) = delete;
    GpuResource(GpuResource&&) noexcept;
    GpuResource& operator=(GpuResource&&) noexcept;

    ///////////////////////////////////////////////////////////
    /// \brief Virtual destructor
    ///
    ///////////////////////////////////////////////////////////
    virtual ~GpuResource();

    ///////////////////////////////////////////////////////////
    /// \brief Get resource handle
    ///
    ///////////////////////////////////////////////////////////
    Handle getHandle() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get resource pointer
    ///
    ///////////////////////////////////////////////////////////
    void* getResource() const;

protected:
    Handle m_handle;            //!< Resource handle
    void* m_resource;           //!< Resource pointer
    priv::DeviceImpl* m_device; //!< Device state
};

class GpuResourceBuilder {
public:
    ///////////////////////////////////////////////////////////
    /// \brief Constructor
    ///
    ///////////////////////////////////////////////////////////
    GpuResourceBuilder();

protected:
    GpuResourceBuilder(RenderDevice* device);
    GpuResourceBuilder(priv::DeviceImpl* device);

    priv::DeviceImpl* m_device; //!< Device state
};

} // namespace ply

#define GPU_RESOURCE_NO_MOVE(X)                                  \
    X(priv::DeviceImpl* device, void* resource, Handle handle) : \
        GpuResource(device, resource, handle) {}

#define GPU_RESOURCE(X)        \
    GPU_RESOURCE_NO_MOVE(X)    \
    X(X&&) noexcept = default; \
    X& operator=(X&&) noexcept = default;