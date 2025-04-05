#include <ply/core/Allocate.h>
#include <ply/ecs/ComponentStore.h>

#include <cstring>

namespace ply {

namespace priv {

///////////////////////////////////////////////////////////
ComponentStore::ComponentStore() : m_start(0),
                                   m_last(0),
                                   m_end(0),
                                   m_typeSize(0),
                                   m_typeAlign(0) {}

///////////////////////////////////////////////////////////
ComponentStore::ComponentStore(size_t size, size_t align) : m_start(0),
                                                                m_last(0),
                                                                m_end(0),
                                                                m_typeSize(size),
                                                                m_typeAlign(align) {
    // Allocate initial space
    m_start = (uint8_t*)ALIGNED_MALLOC_DBG(8 * size, align);
    m_last = m_start;

    m_end = m_start + 8 * size;
}

///////////////////////////////////////////////////////////
ComponentStore::~ComponentStore() {
    if (m_start)
        ALIGNED_FREE_DBG(m_start);

    m_start = 0;
    m_end = 0;
    m_last = 0;
}

///////////////////////////////////////////////////////////
ComponentStore::ComponentStore(const ComponentStore& other) : m_start(0),
                                                              m_last(0),
                                                              m_end(0),
                                                              m_typeSize(other.m_typeSize),
                                                              m_typeAlign(other.m_typeAlign) {
    // Reserve own memory
    reserve((size_t)(other.m_end - other.m_start) / m_typeSize);

    // Copy all elements
    size_t size = (size_t)(other.m_last - other.m_start);
    memcpy(m_start, other.m_start, size);

    // Update last pointer
    m_last = m_start + size;
}

ComponentStore& ComponentStore::operator=(const ComponentStore& other) {
    if (this != &other) {
        // Clear own
        if (m_start) {
            ALIGNED_FREE_DBG(m_start);

            m_start = 0;
            m_last = 0;
            m_end = 0;
            m_typeSize = other.m_typeSize;
            m_typeAlign = other.m_typeAlign;
        }

        // Reserve own memory if capacity is different
        size_t otherCap = (size_t)(other.m_end - other.m_start);
        if (otherCap != (size_t)(m_end - m_start))
            reserve(otherCap / m_typeSize);

        // Copy all elements
        size_t size = (size_t)(other.m_last - other.m_start);
        memcpy(m_start, other.m_start, size);

        // Update last pointer
        m_last = m_start + size;
    }

    return *this;
}

///////////////////////////////////////////////////////////
ComponentStore::ComponentStore(ComponentStore&& other) noexcept : m_start(other.m_start),
                                                                  m_last(other.m_last),
                                                                  m_end(other.m_end),
                                                                  m_typeSize(other.m_typeSize),
                                                                  m_typeAlign(other.m_typeAlign) {
    other.m_start = 0;
    other.m_end = 0;
    other.m_last = 0;
    other.m_typeSize = 0;
    other.m_typeAlign = 0;
}

///////////////////////////////////////////////////////////
ComponentStore& ComponentStore::operator=(ComponentStore&& other) noexcept {
    if (&other != this) {
        m_start = other.m_start;
        m_last = other.m_last;
        m_end = other.m_end;
        m_typeSize = other.m_typeSize;
        m_typeAlign = other.m_typeAlign;

        other.m_start = 0;
        other.m_end = 0;
        other.m_last = 0;
        other.m_typeSize = 0;
        other.m_typeAlign = 0;
    }

    return *this;
}

///////////////////////////////////////////////////////////
void* ComponentStore::push(void* data, size_t instances) {
    // Double space if out
    if (m_last + instances * m_typeSize > m_end)
        reserve((m_end - m_start) / m_typeSize * 2);

    // Keep start of new section
    void* section = m_last;

    // Copy data into array (i forgot why i made a loop for this, but it doesn't work if its a single memcpy() so dont change ig)
    for (size_t i = 0; i < instances; ++i, m_last += m_typeSize)
        memcpy(m_last, data, m_typeSize);

    return section;
}

///////////////////////////////////////////////////////////
void ComponentStore::remove(size_t index) {
    // Copy last into target index
    memcpy(m_start + index * m_typeSize, m_last - m_typeSize, m_typeSize);

    // Decrement last
    m_last -= m_typeSize;
}

///////////////////////////////////////////////////////////
void* ComponentStore::data(size_t index) const {
    return m_start + index * m_typeSize;
}

///////////////////////////////////////////////////////////
void ComponentStore::reserve(size_t size) {
    uint8_t* start = m_start;
    size_t prevSize = (size_t)(m_last - m_start);  // in bytes
    size_t prevCap = (size_t)(m_end - m_start);    // in bytes
    size_t newCap = size * m_typeSize;               // in bytes

    if (prevCap != size) {
        m_start = (uint8_t*)ALIGNED_MALLOC_DBG(newCap, m_typeAlign);
        m_last = m_start + prevSize;
        m_end = m_start + newCap;

        if (start) {
            // Copy previous data if it exists
            memcpy(m_start, start, prevSize);
        }

        // Free prev memory
        ALIGNED_FREE_DBG(start);
    }
}

///////////////////////////////////////////////////////////
size_t ComponentStore::size() const {
    return (size_t)(m_last - m_end) / m_typeSize;
}

///////////////////////////////////////////////////////////
size_t ComponentStore::getTypeSize() const {
    return m_typeSize;
}

}  // namespace priv

}  // namespace ply
