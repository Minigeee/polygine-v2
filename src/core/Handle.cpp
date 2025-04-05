#include <ply/core/Handle.h>

namespace ply {

///////////////////////////////////////////////////////////
Handle::Handle() : m_index(0), m_counter(0) {}

///////////////////////////////////////////////////////////
Handle::Handle(uint32_t index, uint32_t counter) : m_index(index), m_counter(counter) {}

///////////////////////////////////////////////////////////
Handle::operator uint32_t() const {
    return (((uint32_t)m_index) << 8) | m_counter;
}

} // namespace ply