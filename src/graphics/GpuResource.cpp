#include <ply/graphics/GpuResource.h>

#include <ply/graphics/RenderDevice.h>

namespace ply {

///////////////////////////////////////////////////////////
GpuResource::GpuResource() :
    m_device(nullptr) {}

///////////////////////////////////////////////////////////
GpuResource::GpuResource(RenderDevice* device) :
    m_device(device->m_device) {}

///////////////////////////////////////////////////////////
GpuResource::GpuResource(priv::DeviceImpl* device) :
    m_device(device) {}

} // namespace ply
