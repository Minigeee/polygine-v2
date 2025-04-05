#include <ply/ecs/Query.h>

#include <ply/ecs/World.h>

namespace ply {

///////////////////////////////////////////////////////////
Query::Query(World* world, QueryFactory* factory) : m_world(world), m_factory(factory) {}

} // namespace ply