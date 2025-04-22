#include <loguru.hpp>

namespace ply {

///////////////////////////////////////////////////////////
template <typename T>
void Buffer::update(const std::vector<T>& data, uint32_t offset) {
    this->update(data.data(), data.size() * sizeof(T), offset);
}

///////////////////////////////////////////////////////////
template <typename T>
uint32_t Buffer::push(const T& data, uint32_t align, bool reset) {
    CHECK_F(
        size() > 0,
        "tried pushing data to uniform buffer that has not been created"
    );

    MapFlag flags = MapFlag::NoOverwrite;

    // Reset if the push will cause overflow
    if (reset || m_offset == 0 || m_offset + sizeof(T) > size()) {
        m_offset = 0;
        flags = MapFlag::Discard;
    }

    // Map the buffer
    void* mapped = this->map(MapMode::Write, flags);
    CHECK_F(mapped != nullptr, "failed to map uniform buffer for writing");

    // Copy data to the mapped memory
    memcpy((uint8_t*)mapped + m_offset, &data, sizeof(T));

    // Unmap the buffer
    this->unmap();

    // Update the offset
    uint32_t prevOffset = m_offset;
    m_offset += sizeof(T);
    m_offset = (m_offset + align - 1) / align * align;

    return prevOffset;
}

///////////////////////////////////////////////////////////
template <typename T>
BufferBuilder& BufferBuilder::data(const std::vector<T>& data) {
    this->data(data.data(), data.size() * sizeof(T));
    return *this;
}

} // namespace ply