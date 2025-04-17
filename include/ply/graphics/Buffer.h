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
    Buffer();

    ///////////////////////////////////////////////////////////
    /// \brief Construct a GPU buffer resource.
    /// \param device The device implementation pointer.
    /// \param resource The native GPU buffer resource.
    /// \param handle Internal handle for resource management.
    ///
    ///////////////////////////////////////////////////////////
    Buffer(priv::DeviceImpl* device, void* resource, Handle handle);

    ///////////////////////////////////////////////////////////
    /// \brief Destructor. Releases the GPU buffer resource.
    ///
    ///////////////////////////////////////////////////////////
    ~Buffer();

    Buffer(Buffer&&) noexcept;
    Buffer& operator=(Buffer&&) noexcept;

    ///////////////////////////////////////////////////////////
    /// \brief Update the buffer contents with raw data.
    /// \param data Pointer to the data to upload.
    /// \param size Size of the data in bytes.
    /// \param offset Offset in bytes to start updating from (default 0).
    ///
    ///////////////////////////////////////////////////////////
    void update(const void* data, size_t size, size_t offset = 0);

    ///////////////////////////////////////////////////////////
    /// \brief Update the buffer contents with a vector of elements.
    /// \tparam T Element type.
    /// \param data Vector of elements to upload.
    /// \param offset Offset in elements to start updating from (default 0).
    ///
    ///////////////////////////////////////////////////////////
    template <typename T>
    void update(const std::vector<T>& data, uint32_t offset = 0);

    ///////////////////////////////////////////////////////////
    /// \brief Map the buffer for CPU access.
    /// \param mode Mapping mode (read, write, or read/write).
    /// \param flags Optional mapping flags.
    /// \return Pointer to mapped memory.
    ///
    ///////////////////////////////////////////////////////////
    void* map(MapMode mode, MapFlag flags = MapFlag::None);

    ///////////////////////////////////////////////////////////
    /// \brief Unmap the buffer from CPU access.
    ///
    ///////////////////////////////////////////////////////////
    void unmap();

    ///////////////////////////////////////////////////////////
    /// \brief Get the size of the buffer in bytes.
    /// \return Buffer size in bytes.
    ///
    ///////////////////////////////////////////////////////////
    uint32_t getSize() const;

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
    ///////////////////////////////////////////////////////////
    /// \brief Construct a buffer builder for a render device.
    /// \param device The render device pointer.
    ///
    ///////////////////////////////////////////////////////////
    BufferBuilder(RenderDevice* device);

    ///////////////////////////////////////////////////////////
    /// \brief Construct a buffer builder for a device implementation.
    /// \param device The device implementation pointer.
    ///
    ///////////////////////////////////////////////////////////
    BufferBuilder(priv::DeviceImpl* device);

    ///////////////////////////////////////////////////////////
    /// \brief Destructor. Cleans up builder resources.
    ///
    ///////////////////////////////////////////////////////////
    ~BufferBuilder();

    BufferBuilder(const BufferBuilder&) = delete;
    BufferBuilder& operator=(const BufferBuilder&) = delete;
    BufferBuilder(BufferBuilder&&) noexcept;
    BufferBuilder& operator=(BufferBuilder&&) noexcept;

    ///////////////////////////////////////////////////////////
    /// \brief Set the resource bind flags for the buffer.
    /// \param bind Resource bind flags (e.g., VertexBuffer, IndexBuffer).
    ///
    ///////////////////////////////////////////////////////////
    BufferBuilder& bind(ResourceBind bind);

    ///////////////////////////////////////////////////////////
    /// \brief Set the usage type for the buffer (e.g., Immutable, Dynamic).
    /// \param usage Resource usage type.
    ///
    ///////////////////////////////////////////////////////////
    BufferBuilder& usage(ResourceUsage usage);

    ///////////////////////////////////////////////////////////
    /// \brief Set the CPU access flags for the buffer.
    /// \param access CPU access flags (e.g., Read, Write).
    ///
    ///////////////////////////////////////////////////////////
    BufferBuilder& access(ResourceAccess access);

    ///////////////////////////////////////////////////////////
    /// \brief Set the size of the buffer in bytes.
    /// \param size Buffer size in bytes.
    ///
    ///////////////////////////////////////////////////////////
    BufferBuilder& size(size_t size);

    ///////////////////////////////////////////////////////////
    /// \brief Set the initial data for the buffer from a vector.
    /// \tparam T Element type.
    /// \param data Vector of elements to upload.
    ///
    ///////////////////////////////////////////////////////////
    template <typename T>
    BufferBuilder& data(const std::vector<T>& data);

    ///////////////////////////////////////////////////////////
    /// \brief Set the initial data for the buffer from raw memory.
    /// \param data Pointer to the data.
    /// \param size Size of the data in bytes.
    ///
    ///////////////////////////////////////////////////////////
    BufferBuilder& data(const void* data, size_t size);

    ///////////////////////////////////////////////////////////
    /// \brief Create the buffer resource with the specified parameters.
    /// \return The created Buffer object.
    ///
    ///////////////////////////////////////////////////////////
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
    ///////////////////////////////////////////////////////////
    /// \brief Default constructor.
    ///
    ///////////////////////////////////////////////////////////
    BufferList() = default;

    ///////////////////////////////////////////////////////////
    /// \brief Construct from an initializer list of Buffer pointers.
    ///
    ///////////////////////////////////////////////////////////
    BufferList(const std::initializer_list<Buffer*>& list);

    ///////////////////////////////////////////////////////////
    /// \brief Add a buffer to the list.
    /// \param buffer Buffer to add.
    ///
    ///////////////////////////////////////////////////////////
    void push(const Buffer& buffer);

    ///////////////////////////////////////////////////////////
    /// \brief Get the raw data pointer array for the buffers.
    /// \return Pointer to the array of buffer resources.
    ///
    ///////////////////////////////////////////////////////////
    void** data() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the number of buffers in the list.
    /// \return Number of buffers.
    ///
    ///////////////////////////////////////////////////////////
    size_t size() const;

private:
    std::vector<void*> m_buffers;
};

} // namespace ply

#include <ply/graphics/Buffer.inl>