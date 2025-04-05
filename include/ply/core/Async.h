#pragma once

#include <ply/core/Handle.h>
#include <ply/core/HandleArray.h>
#include <ply/core/Time.h>

#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>

namespace ply {

///////////////////////////////////////////////////////////
/// \brief Async utilities
///
///////////////////////////////////////////////////////////
class Async {
public:
    ~Async();

    ///////////////////////////////////////////////////////////
    /// \brief Call the given callback function on a timeout delay
    ///
    /// \param callback The callback function
    /// \param delay The delay to wait before calling the function
    ///
    /// \return A handle that can be used to cancel the timeout
    ///
    ///////////////////////////////////////////////////////////
    static Handle onTimeout(const std::function<void()>& callback, Time delay);

    ///////////////////////////////////////////////////////////
    /// \brief Cancel timeout
    ///
    /// \param handle The handle of the timeout to cancel
    ///
    ///////////////////////////////////////////////////////////
    static void cancelTimeout(Handle handle);

private:
    struct TimerData {
        std::thread m_thread;
        std::condition_variable m_cv;
        std::mutex m_mutex;
        std::atomic_bool m_active;
    };

private:
    HandleArray<std::shared_ptr<TimerData>> m_timers;
    std::mutex m_mutex;

    static Async s_singleton;
};

}