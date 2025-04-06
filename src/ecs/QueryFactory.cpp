#include <ply/ecs/QueryFactory.h>

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
    uint32_t entityIdx
)
    : QueryAccessor(world, group, _id, entityIdx),
      index(_index) {}

///////////////////////////////////////////////////////////
uint32_t QueryDescriptor::getHash() const {
    // Use group hash as base
    uint32_t base = entityGroupHash(m_include);

    // Add exclude
    for (auto type : m_exclude)
        base ^= type.hash_code();

    return base;
}

///////////////////////////////////////////////////////////
void QueryDescriptor::addInclude(const std::type_index& type) {
    // Check if the list already has type
    bool alreadyContains = false;
    for (auto t : m_include)
        alreadyContains |= t == type;

    // Only add if it doesn't
    if (!alreadyContains)
        m_include.push_back(type);
}

///////////////////////////////////////////////////////////
void QueryDescriptor::addExclude(const std::type_index& type) {
    // Check if the list already has type
    bool alreadyContains = false;
    for (auto t : m_exclude)
        alreadyContains |= t == type;

    // Only add if it doesn't
    if (!alreadyContains)
        m_exclude.push_back(type);
}

///////////////////////////////////////////////////////////
QueryFactory::QueryFactory(World* world) : m_world(world) {}

///////////////////////////////////////////////////////////
QueryFactory& QueryFactory::lock(std::mutex& mutex) {
    m_mutexes.push_back(&mutex);
    return *this;
}

///////////////////////////////////////////////////////////
QueryFactory& QueryFactory::match(const TypeSet& include) {
    // Clear includes
    m_include.clear();

    // Add all from set
    for (auto type : include.getSet())
        m_include.push_back(type);

    return *this;
}

///////////////////////////////////////////////////////////
QueryFactory& QueryFactory::exclude(const TypeSet& exclude) {
    // Clear excludes
    m_exclude.clear();

    // Add all from set
    for (auto type : exclude.getSet())
        m_exclude.push_back(type);

    return *this;
}

///////////////////////////////////////////////////////////
Query QueryFactory::compile() {
    // Register query
    QueryFactory* q = m_world->registerQuery(this);
    return Query(m_world, q);
}

} // namespace ply
