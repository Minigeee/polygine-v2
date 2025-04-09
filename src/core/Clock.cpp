#include <ply/core/Clock.h>

using namespace std::chrono;

namespace ply {

///////////////////////////////////////////////////////////
Clock::Clock() :
    m_startTime(high_resolution_clock::now()) {}

///////////////////////////////////////////////////////////
Time Clock::now() {
    return Time(time_point_cast<microseconds>(high_resolution_clock::now())
                    .time_since_epoch()
                    .count());
}

///////////////////////////////////////////////////////////
Time Clock::restart() {
    // Get the elapsed duration
    auto now = high_resolution_clock::now();
    auto elapsed = high_resolution_clock::now() - m_startTime;

    // Reset the clock
    m_startTime = now;

    // Return the time
    return Time(duration_cast<microseconds>(elapsed).count());
}

///////////////////////////////////////////////////////////
Time Clock::getElapsedTime() const {
    // Get time elapsed since last start time
    return Time(
        duration_cast<microseconds>(high_resolution_clock::now() - m_startTime)
            .count()
    );
}

} // namespace ply