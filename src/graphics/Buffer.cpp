#include <ply/graphics/Buffer.h>

#include "RenderImpl.h"
#include <ply/core/PoolAllocator.h>
#include <ply/graphics/RenderDevice.h>

#define BUFFER(x) static_cast<Diligent::IBuffer*>(x)

namespace ply {

///////////////////////////////////////////////////////////
Buffer::Buffer() :
    GpuResource(),
    m_mapped(nullptr) {}

///////////////////////////////////////////////////////////
Buffer::Buffer(priv::DeviceImpl* device, void* resource, Handle handle) :
    GpuResource(device, resource, handle),
    m_mapped(nullptr) {}

///////////////////////////////////////////////////////////
Buffer::~Buffer() {
    if (m_device && m_resource) {
        m_device->m_buffers.remove(m_handle);
    }

    if (m_mapped) {
        unmap();
    }
}

///////////////////////////////////////////////////////////
Buffer::Buffer(Buffer&& other) noexcept :
    GpuResource(std::move(other)),
    m_mapped(std::exchange(other.m_mapped, nullptr)),
    m_mode(other.m_mode) {}

///////////////////////////////////////////////////////////
Buffer& Buffer::operator=(Buffer&& other) noexcept {
    if (&other != this) {
        GpuResource::operator=(std::move(other));
        m_mapped = std::exchange(other.m_mapped, nullptr);
        m_mode = other.m_mode;
    }
    return *this;
}

///////////////////////////////////////////////////////////
void Buffer::update(const void* data, size_t size, size_t offset) {
    m_device->m_deviceContext->UpdateBuffer(
        BUFFER(m_resource),
        offset,
        size,
        data,
        m_device->m_transitionMode
    );
}

///////////////////////////////////////////////////////////
void* Buffer::map(MapMode mode, MapFlag flags) {
    void* mapped = nullptr;
    m_device->m_deviceContext->MapBuffer(
        BUFFER(m_resource),
        static_cast<MAP_TYPE>(mode),
        static_cast<MAP_FLAGS>(flags),
        mapped
    );

    m_mode = mode;
    m_mapped = mapped;

    return mapped;
}

///////////////////////////////////////////////////////////
void Buffer::unmap() {
    if (m_mapped) {
        m_device->m_deviceContext->UnmapBuffer(
            BUFFER(m_resource),
            static_cast<MAP_TYPE>(m_mode)
        );
        m_mapped = nullptr;
    }
}

///////////////////////////////////////////////////////////
uint32_t Buffer::getSize() const {
    return BUFFER(m_resource)->GetDesc().Size;
}

///////////////////////////////////////////////////////////
BufferBuilder::BufferBuilder(RenderDevice* device) :
    BufferBuilder(device->m_device) {}

///////////////////////////////////////////////////////////
BufferBuilder::BufferBuilder(priv::DeviceImpl* device) :
    GpuResourceBuilder(device) {
    m_desc = Pool<priv::BufferDesc>::alloc();
}

///////////////////////////////////////////////////////////
BufferBuilder::~BufferBuilder() {
    if (m_desc) {
        Pool<priv::BufferDesc>::free(m_desc);
    }
}

///////////////////////////////////////////////////////////
BufferBuilder::BufferBuilder(BufferBuilder&& other) noexcept :
    m_desc(std::exchange(other.m_desc, nullptr)) {}

///////////////////////////////////////////////////////////
BufferBuilder& BufferBuilder::operator=(BufferBuilder&& other) noexcept {
    if (this != &other) {
        m_desc = std::exchange(other.m_desc, nullptr);
    }
    return *this;
}

///////////////////////////////////////////////////////////
BufferBuilder& BufferBuilder::bind(ResourceBind bind) {
    m_desc->BindFlags = static_cast<BIND_FLAGS>(bind);
    return *this;
}

///////////////////////////////////////////////////////////
BufferBuilder& BufferBuilder::usage(ResourceUsage usage) {
    m_desc->Usage = static_cast<USAGE>(usage);
    return *this;
}

///////////////////////////////////////////////////////////
BufferBuilder& BufferBuilder::access(ResourceAccess access) {
    m_desc->CPUAccessFlags = static_cast<CPU_ACCESS_FLAGS>(access);
    return *this;
}

///////////////////////////////////////////////////////////
BufferBuilder& BufferBuilder::size(size_t size) {
    m_desc->Size = size;
    return *this;
}

///////////////////////////////////////////////////////////
BufferBuilder& BufferBuilder::data(const void* data, size_t size) {
    // Set buffer data
    m_desc->Data.DataSize = size;
    m_desc->Data.pData = data;

    // Change buffer size if needed
    if (size > m_desc->Size) {
        m_desc->Size = size;
    }

    return *this;
}

///////////////////////////////////////////////////////////
Buffer BufferBuilder::create() {
    // Create buffer
    RefCntAutoPtr<IBuffer> buffer;
    m_device->m_renderDevice->CreateBuffer(
        *m_desc,
        m_desc->Data.DataSize > 0 ? &m_desc->Data : nullptr,
        &buffer
    );

    // Register buffer
    Handle handle = m_device->m_buffers.push(buffer);

    return Buffer(m_device, buffer, handle);
}

///////////////////////////////////////////////////////////
BufferList::BufferList(const std::initializer_list<Buffer*>& list) {
    for (auto buffer : list) {
        m_buffers.push_back(buffer->getResource());
    }
}

///////////////////////////////////////////////////////////
void BufferList::push(const Buffer& buffer) {
    m_buffers.push_back(buffer.getResource());
}

///////////////////////////////////////////////////////////
void** BufferList::data() const {
    return (void**)m_buffers.data();
}

///////////////////////////////////////////////////////////
size_t BufferList::size() const {
    return m_buffers.size();
}

} // namespace ply