#include <ply/core/Async.h>

namespace ply {

///////////////////////////////////////////////////////////
Async Async::s_singleton;

///////////////////////////////////////////////////////////
Async::~Async() {
    // Cancel the rest of these
    auto timers = m_timers.data(); // Create copy bc the main one will be removed from
    for (size_t i = 0; i < timers.size(); ++i) {
        auto timer = timers[i];
        timer->m_active = false;
        timer->m_cv.notify_one();
    }
}

///////////////////////////////////////////////////////////
Handle Async::onTimeout(const std::function<void()>& callback, Time delay) {
    // Create timer
    std::shared_ptr<TimerData> timer = std::make_shared<TimerData>();

    // Add to handle array
    Handle handle;
    {
        std::lock_guard<std::mutex> lock(s_singleton.m_mutex);
        handle = s_singleton.m_timers.push(timer);
    }

    // Pointers to pass to thread
    auto timers = &s_singleton.m_timers;
    std::mutex* timerMutex = &s_singleton.m_mutex;

    // Create thread
    timer->m_active = true;
    timer->m_thread = std::thread([=]() {
        {
            // Wait delay
            std::unique_lock<std::mutex> lock(timer->m_mutex);
            timer->m_cv.wait_for(lock, std::chrono::milliseconds(delay.milliseconds()));

            // Callback if still active
            if (timer->m_active && callback)
                callback();
        }

        // Remove timer
        std::lock_guard<std::mutex> timerLock(*timerMutex);
        timers->remove(handle);
    });
    // Detach
    timer->m_thread.detach();

    return handle;
}

///////////////////////////////////////////////////////////
void Async::cancelTimeout(Handle handle) {
    // Quit if invalid handle
    if (!s_singleton.m_timers.isValid(handle))
        return;

    // Set as inactive
    std::shared_ptr<TimerData> timer = NULL;
    {
        std::lock_guard<std::mutex> lock(s_singleton.m_mutex);
        timer = s_singleton.m_timers[handle];
    }

    timer->m_active = false;
    timer->m_cv.notify_one();
}

}