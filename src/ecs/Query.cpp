#include <ply/ecs/Query.h>

#include <ply/ecs/World.h>

namespace ply {

///////////////////////////////////////////////////////////
Query::Query(World* world, QueryFactory* factory) : m_world(world), m_factory(factory) {}

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