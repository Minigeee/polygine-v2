#include <ply/graphics/GpuResource.h>

#include <ply/graphics/RenderDevice.h>
#include <utility>

namespace ply {

///////////////////////////////////////////////////////////
GpuResource::GpuResource() :
    m_device(nullptr),
    m_resource(nullptr) {}

///////////////////////////////////////////////////////////
GpuResource::GpuResource(RenderDevice* device) :
    m_device(device->m_device),
    m_resource(nullptr) {}

///////////////////////////////////////////////////////////
GpuResource::GpuResource(
    priv::DeviceImpl* device,
    void* resource,
    Handle handle
) :
    m_device(device),
    m_resource(resource),
    m_handle(handle) {}

///////////////////////////////////////////////////////////
GpuResource::~GpuResource() {
    m_resource = nullptr;
}

///////////////////////////////////////////////////////////
GpuResource::GpuResource(GpuResource&& other) noexcept :
    m_resource(std::exchange(other.m_resource, nullptr)),
    m_handle(other.m_handle),
    m_device(other.m_device) {}

///////////////////////////////////////////////////////////
GpuResource& GpuResource::operator=(GpuResource&& other) noexcept {
    if (this != &other) {
        m_device = other.m_device;
        m_resource = other.m_resource;
        m_handle = other.m_handle;
        other.m_device = nullptr;
        other.m_resource = nullptr;
    }
    return *this;
}

///////////////////////////////////////////////////////////
Handle GpuResource::getHandle() const {
    return m_handle;
}

///////////////////////////////////////////////////////////
void* GpuResource::getResource() const {
    return m_resource;
}

///////////////////////////////////////////////////////////
GpuResourceBuilder::GpuResourceBuilder() :
    m_device(nullptr) {}

///////////////////////////////////////////////////////////
GpuResourceBuilder::GpuResourceBuilder(RenderDevice* device) :
    m_device(device->m_device) {}

///////////////////////////////////////////////////////////
GpuResourceBuilder::GpuResourceBuilder(priv::DeviceImpl* device) :
    m_device(device) {}

} // namespace ply
