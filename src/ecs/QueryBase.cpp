#include <ply/ecs/QueryBase.h>

#include <ply/ecs/Query.h>
#include <ply/ecs/World.h>

namespace ply {

///////////////////////////////////////////////////////////
QueryAccessor::QueryAccessor(World* world, EntityGroup* group, EntityId id, uint32_t entityIdx)
    : m_world(world),
      m_group(group),
      m_entityIdx(entityIdx),
      id(id) {}

///////////////////////////////////////////////////////////
QueryAccessor QueryAccessor::getEntity(EntityId id) const {
    // Assuming that mutexes are already locked

    World::EntityData& data = m_world->m_entities[id];
    return QueryAccessor(m_world, m_world->m_groups[data.m_group].get(), this->id, data.m_index);
}

///////////////////////////////////////////////////////////
QueryIterator::QueryIterator(
    EntityId _id,
    uint32_t _index,
    World* world,
    EntityGroup* group,
    uint32_t entityIdx,
    float _dt
)
    : QueryAccessor(world, group, _id, entityIdx),
      index(_index),
      dt(_dt) {}

///////////////////////////////////////////////////////////
uint32_t QueryBase::getHash() const {
    // Use group hash as base
    uint32_t base = entityGroupHash(m_include);

    // Add exclude
    for (auto type : m_exclude)
        base ^= type.hash_code();

    return base;
}

///////////////////////////////////////////////////////////
void QueryBase::addInclude(const std::type_index& type) {
    // Check if the list already has type
    bool alreadyContains = false;
    for (auto t : m_include)
        alreadyContains |= t == type;

    // Only add if it doesn't
    if (!alreadyContains)
        m_include.push_back(type);
}

///////////////////////////////////////////////////////////
void QueryBase::addExclude(const std::type_index& type) {
    // Check if the list already has type
    bool alreadyContains = false;
    for (auto t : m_exclude)
        alreadyContains |= t == type;

    // Only add if it doesn't
    if (!alreadyContains)
        m_exclude.push_back(type);
}

} // namespace ply
