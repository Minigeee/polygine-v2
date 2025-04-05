#include <ply/core/TypeSet.h>

namespace ply {

///////////////////////////////////////////////////////////
const HashSet<std::type_index>& TypeSet::getSet() const {
    return m_set;
}

}  // namespace ply
