#pragma once

#include <ply/core/Time.h>
#include <chrono>

namespace ply {

///////////////////////////////////////////////////////////
/// \brief A class for measuring elapsed time
///
///////////////////////////////////////////////////////////
class Clock {
public:
    ///////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// The clock automatically starts as soon as the clock
    /// is constructed.
    ///
    ///////////////////////////////////////////////////////////
    Clock();
    
    ///////////////////////////////////////////////////////////
    /// \brief Get current time
    ///
    /// \return A Time containing the current time
    ///
    ///////////////////////////////////////////////////////////
    static Time now();

    ///////////////////////////////////////////////////////////
    /// \brief Restart the clock at zero
    ///
    /// Restarts the clock at zero, and return the time elapsed
    /// since the last time the clock was restart or constructed.
    ///
    /// \return A Time containing the time elapsed since the last restart
    ///
    ///////////////////////////////////////////////////////////
    Time restart();

    ///////////////////////////////////////////////////////////
    /// \brief Get the time elapsed since the last restart
    ///
    /// \return A Time containing the time elapsed since the last restart
    ///
    ///////////////////////////////////////////////////////////
    Time getElapsedTime() const;

private:
    std::chrono::time_point<std::chrono::high_resolution_clock>
        m_startTime; //!< A time point representing the start time of the clock
};

}

///////////////////////////////////////////////////////////
/// \class ply::Clock
/// \ingroup Core
///
/// ply::Clock is a class that can measure elapsed time.
/// The clock object will automatically start measuring as soon
/// as it is created. Whenever restart() is called, the clock
/// return the elapsed time since its last restart, and it will
/// start measuring from 0 again. Use getElapsedTime() to
/// get the elapsed time without restarting the clock.
///
/// This clock is a wrapper around std::chrono::high_resolution_clock.
///
/// Usage example:
/// \code
///
/// using namespace ply;
///
/// // The clock creates its start point for measuring time
/// // as soon as it is created
/// Clock clock;
///
/// for (int i = 0; i < 10000; ++i)
///		std::cout << "Hello World!\n";
///
/// // Print the amount of elapsed time and restart the clock
/// std::cout << clock.restart().seconds() << "s\n";
///
/// std::cout << "Hello World!\n";
///
/// // Print the amount of elapsed time, but don't restart the clock
/// std::cout << clock.getElapsedTime().seconds() << "s\n";
///
/// \endcode
///
///////////////////////////////////////////////////////////