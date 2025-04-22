#pragma once

#include <ply/core/Handle.h>

namespace ply {

class RenderDevice;
class Pipeline;
class ResourceBinding;

namespace priv {
    struct DeviceImpl;
    struct ResourceStateListImpl;
} // namespace priv

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

///////////////////////////////////////////////////////////
/// \brief Utility class to hold list of resource states
///
///////////////////////////////////////////////////////////
/* class ResourceStateList {
public:
    ResourceStateList();
    ResourceStateList(
        std::initializer_list<std::pair<const GpuResource*, ResourceState>> list
    );

    ///////////////////////////////////////////////////////////
    /// \brief Add a resource state to the list
    ///
    ///////////////////////////////////////////////////////////
    void push(const Buffer& resource, ResourceState state);

    ///////////////////////////////////////////////////////////
    /// \brief Get the size of the resource states list
    ///
    ///////////////////////////////////////////////////////////
    size_t size() const;

private:
    std::unique_ptr<priv::ResourceStateListImpl>
        m_impl; //!< Implementation details
}; */

} // namespace ply

#define GPU_RESOURCE(X)                                          \
    X() :                                                        \
        GpuResource() {}                                         \
    X(priv::DeviceImpl* device, void* resource, Handle handle) : \
        GpuResource(device, resource, handle) {}                 \
    X(X&&) noexcept;                                             \
    X& operator=(X&&) noexcept;

#define GPU_RESOURCE_MOVE_DEFS(x, r_array)            \
    x::x(x&& other) noexcept :                        \
        GpuResource(std::move(other)) {}              \
    x& x::operator=(x&& other) noexcept {             \
        if (this != &other) {                         \
            if (m_device && m_resource) {             \
                m_device->r_array.remove(m_handle);   \
            }                                         \
            GpuResource::operator=(std::move(other)); \
        }                                             \
        return *this;                                 \
    }