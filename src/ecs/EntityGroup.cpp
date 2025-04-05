#include <ply/ecs/EntityGroup.h>

namespace ply {

///////////////////////////////////////////////////////////
EntityGroupId entityGroupHash(const std::vector<std::type_index>& ids) {
    if (ids.size() < 0)
        return 0;

    EntityGroupId hash = ids[0].hash_code();
    for (size_t i = 1; i < ids.size(); ++i)
        hash *= ids[i].hash_code();

    return hash;
}

} // namespace ply