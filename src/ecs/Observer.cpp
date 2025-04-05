#include <ply/ecs/Observer.h>

namespace ply {

///////////////////////////////////////////////////////////
Observer& Observer::lock(std::mutex& mutex) {
    m_mutexes.push_back(&mutex);
    return *this;
}

///////////////////////////////////////////////////////////
Observer& Observer::match(const TypeSet& include) {
    // Clear includes
    m_include.clear();

    // Add all from set
    for (auto type : include.getSet())
        m_include.push_back(type);

    return *this;
}

///////////////////////////////////////////////////////////
Observer& Observer::exclude(const TypeSet& exclude) {
    // Clear excludes
    m_exclude.clear();

    // Add all from set
    for (auto type : exclude.getSet())
        m_exclude.push_back(type);

    return *this;
}

} // namespace ply