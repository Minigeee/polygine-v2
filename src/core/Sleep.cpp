#include <ply/core/Sleep.h>

#include <chrono>
#include <thread>

namespace ply {

void sleep(Time time) {
    uint64_t us = time.microseconds();

    if (us > 0) {
        // Use C++11 sleep_for
        std::this_thread::sleep_for(std::chrono::microseconds(us));
    }
}

void sleep(float seconds) {
    if (seconds > 0.0f) {
        // Use C++11 sleep_for
        std::this_thread::sleep_for(
            std::chrono::microseconds(Time::fromSeconds(seconds).microseconds())
        );
    }
}

void sleep(double seconds) {
    sleep((float)seconds);
}

void sleep(int milliseconds) {
    if (milliseconds > 0) {
        // Use C++11 sleep_for
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }
}

void sleep(unsigned int milliseconds) {
    sleep((int)milliseconds);
}

}