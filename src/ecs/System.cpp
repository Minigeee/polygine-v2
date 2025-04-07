#include <ply/ecs/System.h>

namespace ply {

///////////////////////////////////////////////////////////
System::System(World* world) : m_world(world) {}

///////////////////////////////////////////////////////////
System& System::lock(std::mutex& mutex) {
    m_mutexes.push_back(&mutex);
    return *this;
}

///////////////////////////////////////////////////////////
System& System::match(const TypeSet& include) {
    // Clear includes
    m_include.clear();

    // Add all from set
    for (auto type : include.getSet())
        m_include.push_back(type);

    return *this;
}

///////////////////////////////////////////////////////////
System& System::exclude(const TypeSet& exclude) {
    // Clear excludes
    m_exclude.clear();

    // Add all from set
    for (auto type : exclude.getSet())
        m_exclude.push_back(type);

    return *this;
}

///////////////////////////////////////////////////////////
System& System::before(System* system) {
    // Check if the dependency is not already added
    if (std::find(m_dependencies.begin(), m_dependencies.end(), system) == m_dependencies.end()) {
        // Add this as a dependency to the other system
        system->m_dependencies.push_back(this);
    }
    return *this;
}

///////////////////////////////////////////////////////////
System& System::after(System* system) {
    // Check if the dependency is not already added
    if (std::find(system->m_dependencies.begin(), system->m_dependencies.end(), this) ==
        system->m_dependencies.end()) {
        // Add other system as a dependency
        m_dependencies.push_back(system);
    }
    return *this;
}

} // namespace ply