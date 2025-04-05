#include <ply/engine/Events.h>

namespace ply {

///////////////////////////////////////////////////////////
EventSystem::~EventSystem() {
    // Poll once more to clear queues
    poll();

    // Cleanup all event queues
    std::lock_guard<std::mutex> lock(m_mutex);
    for (size_t i = 0; i < m_lifecycleHandlers.size(); ++i)
        m_lifecycleHandlers[i].m_cleanup();
}

///////////////////////////////////////////////////////////
void EventSystem::poll() {
    std::lock_guard<std::mutex> lock(m_mutex);
    for (size_t i = 0; i < m_lifecycleHandlers.size(); ++i)
        m_lifecycleHandlers[i].m_processEvents();
}

}