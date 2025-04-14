#pragma once

#include <ply/graphics/GpuResource.h>
#include <ply/graphics/Types.h>

#include <initializer_list>
#include <vector>

namespace ply {

namespace priv {
    struct BufferDesc;
}

///////////////////////////////////////////////////////////
/// \brief A gpu buffer
///
///////////////////////////////////////////////////////////
class Buffer : public GpuResource {
public:
    Buffer(priv::DeviceImpl* device, void* resource, Handle handle);

    ~Buffer();
    Buffer(Buffer&&) noexcept;
    Buffer& operator=(Buffer&&) noexcept;

    void update(const void* data, size_t size, size_t offset = 0);

    ///////////////////////////////////////////////////////////
    /// \brief Update data in a buffer
    ///
    /// The buffer must have been created and its size
    /// must be bigger than the new data size
    ///
    /// \param data The new data to replace in the buffer
    /// \param offset The destination offset to copy the data to (measured in
    /// number of elements)
    ///
    ///////////////////////////////////////////////////////////
    template <typename T>
    void update(const std::vector<T>& data, uint32_t offset = 0);

    void* map(MapMode mode, MapFlag flags = MapFlag::None);

    void unmap();

private:
    void* m_mapped; //!< Current mapped pointer
    MapMode m_mode; //!< Current map mode
};

///////////////////////////////////////////////////////////
/// \brief A gpu buffer builder
///
///////////////////////////////////////////////////////////
class BufferBuilder : public GpuResourceBuilder {
public:
    BufferBuilder(RenderDevice* device);
    BufferBuilder(priv::DeviceImpl* device);
    ~BufferBuilder();

    BufferBuilder(const BufferBuilder&) = delete;
    BufferBuilder& operator=(const BufferBuilder&) = delete;
    BufferBuilder(BufferBuilder&&) noexcept;
    BufferBuilder& operator=(BufferBuilder&&) noexcept;

    BufferBuilder& bind(ResourceBind bind);

    BufferBuilder& usage(ResourceUsage usage);

    BufferBuilder& access(ResourceAccess access);

    BufferBuilder& size(size_t size);

    template <typename T>
    BufferBuilder& data(const std::vector<T>& data);

    BufferBuilder& data(const void* data, size_t size);

    Buffer create();

private:
    priv::BufferDesc* m_desc;
};

///////////////////////////////////////////////////////////
/// \brief Utility class for holding a list of buffers
///
///////////////////////////////////////////////////////////
class BufferList {
public:
    BufferList() = default;
    BufferList(const std::initializer_list<Buffer*>& list);

    void push(const Buffer& buffer);

    void** data() const;

    size_t size() const;

private:
    std::vector<void*> m_buffers;
};

} // namespace ply

#include <ply/graphics/Buffer.inl>