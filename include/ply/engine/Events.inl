namespace ply {

namespace priv {

    ///////////////////////////////////////////////////////////
    template <typename E>
    inline Handle EventDispatcherImpl<E>::addListener(std::function<void(const E&)>&& func) {
        // Add the listener and return the handle
        return m_listeners.push(std::move(func));
    }

    ///////////////////////////////////////////////////////////
    template <typename E> inline void EventDispatcherImpl<E>::removeListener(Handle handle) {
        m_listeners.remove(handle);
    }

    ///////////////////////////////////////////////////////////
    template <typename E> inline void EventDispatcherImpl<E>::sendEvent(const E& event) {
        // Iterate all listeners and call the function
        const std::vector<std::function<void(const E&)>>& listeners = m_listeners.data();
        for (uint32_t i = 0; i < listeners.size(); ++i)
            listeners[i](event);
    }

}

///////////////////////////////////////////////////////////
template <typename... Es>
template <typename E>
Handle EventDispatcher<Es...>::addListener(std::function<void(const E&)>&& func) {
    WriteLock lock(m_mutex);
    return priv::EventDispatcherImpl<E>::addListener(
        std::forward<std::function<void(const E&)>>(func)
    );
}

///////////////////////////////////////////////////////////
template <typename... Es>
template <typename E>
void EventDispatcher<Es...>::removeListener(Handle handle) {
    WriteLock lock(m_mutex);
    priv::EventDispatcherImpl<E>::removeListener(handle);
}

///////////////////////////////////////////////////////////
template <typename... Es>
template <typename E>
void EventDispatcher<Es...>::sendEvent(const E& event) {
    ReadLock lock(m_mutex);
    priv::EventDispatcherImpl<E>::sendEvent(event);
}

///////////////////////////////////////////////////////////
template <typename E, typename Func> Handle EventSystem::addListener(Func&& func) {
    // Create event queue if needed
    priv::EventQueue<E>& queue = getOrCreateEventQueue<E>();

    // Add listener to queue
    std::lock_guard<std::mutex> lock(queue.m_mutex);
    return queue.m_listeners.push(std::forward<Func>(func));
}

///////////////////////////////////////////////////////////
template <typename E> void EventSystem::removeListener(Handle handle) {
    // Create event queue if needed
    priv::EventQueue<E>& queue = getOrCreateEventQueue<E>();

    // Remove listener from queue
    std::lock_guard<std::mutex> lock(queue.m_mutex);
    queue.m_listeners.remove(handle);
}

///////////////////////////////////////////////////////////
template <typename E> void EventSystem::sendEvent(const E& event) {
    // Create event queue if needed
    priv::EventQueue<E>* queue = nullptr;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_queues.find(typeid(E));
        if (it != m_queues.end())
            queue = static_cast<priv::EventQueue<E>*>(it->second);
    }

    // Add event to queue
    if (queue) {
        std::lock_guard<std::mutex> lock(queue->m_mutex);
        queue->m_events.push_back(event);
    }
}

///////////////////////////////////////////////////////////
template <typename E> priv::EventQueue<E>& EventSystem::getOrCreateEventQueue() {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = m_queues.find(typeid(E));
    if (it != m_queues.end())
        return *static_cast<priv::EventQueue<E>*>(it->second);

    // Create queue
    priv::EventQueue<E>* queue = new priv::EventQueue<E>();
    m_queues[typeid(E)] = queue;

    // Add lifecycle handlers
    priv::EventQueueLifecycleHandler handlers;
    handlers.m_processEvents = [queue]() {
        // Lock
        std::lock_guard<std::mutex> lock(queue->m_mutex);

        auto& events = queue->m_events;
        auto& listeners = queue->m_listeners.data();

        for (const auto& e : events) {
            for (size_t i = 0; i < listeners.size(); ++i)
                listeners[i](e);
        }

        // Clear events
        events.clear();
    };
    handlers.m_cleanup = [queue]() { delete queue; };

    m_lifecycleHandlers.push_back(std::move(handlers));

    return *queue;
}

} // namespace ply
