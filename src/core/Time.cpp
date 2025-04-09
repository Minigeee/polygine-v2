#include <ply/core/Time.h>

#include <chrono>

namespace ply {

///////////////////////////////////////////////////////////
Time::Time() :
    m_microseconds(0) {}

///////////////////////////////////////////////////////////
Time::Time(int64_t micro) :
    m_microseconds(micro) {}

///////////////////////////////////////////////////////////
Time Time::fromMinutes(double minutes) {
    return Time((int64_t)(minutes * 60000000));
}

///////////////////////////////////////////////////////////
Time Time::fromSeconds(double seconds) {
    return Time((int64_t)((double)seconds * 1000000));
}

///////////////////////////////////////////////////////////
Time Time::fromMilliseconds(int64_t milliseconds) {
    return Time((int64_t)milliseconds * 1000);
}

///////////////////////////////////////////////////////////
Time Time::fromMicroseconds(int64_t microseconds) {
    return Time(microseconds);
}

///////////////////////////////////////////////////////////
double Time::minutes() const {
    return (double)(m_microseconds * 0.000001 / 60.0);
}

///////////////////////////////////////////////////////////
double Time::seconds() const {
    return (double)(m_microseconds * 0.000001);
}

///////////////////////////////////////////////////////////
int64_t Time::milliseconds() const {
    return (int32_t)(m_microseconds * 0.001);
}

///////////////////////////////////////////////////////////
int64_t Time::microseconds() const {
    return m_microseconds;
}

///////////////////////////////////////////////////////////
Time operator-(Time a, Time b) {
    return Time(a.microseconds() - b.microseconds());
}

///////////////////////////////////////////////////////////
Time operator-=(Time& a, Time b) {
    a = Time(a.microseconds() - b.microseconds());
    return a;
}

///////////////////////////////////////////////////////////
Time operator+(Time a, Time b) {
    return Time(a.microseconds() + b.microseconds());
}

///////////////////////////////////////////////////////////
Time operator+=(Time& a, Time b) {
    a = Time(a.microseconds() + b.microseconds());
    return a;
}

///////////////////////////////////////////////////////////
Time operator*(Time a, int64_t b) {
    return Time(a.microseconds() * b);
}

///////////////////////////////////////////////////////////
Time operator*(int64_t a, Time b) {
    return Time(a * b.microseconds());
}

///////////////////////////////////////////////////////////
Time& operator*=(Time& a, int64_t b) {
    a = Time(a.microseconds() * b);
    return a;
}

///////////////////////////////////////////////////////////
Time operator*(Time a, double b) {
    return Time((int64_t)(a.microseconds() * b));
}

///////////////////////////////////////////////////////////
Time operator*(double a, Time b) {
    return Time((int64_t)(a * b.microseconds()));
}

///////////////////////////////////////////////////////////
Time& operator*=(Time& a, double b) {
    a = Time((int64_t)(a.microseconds() * b));
    return a;
}

///////////////////////////////////////////////////////////
Time operator/(Time a, int64_t b) {
    return Time(a.microseconds() / b);
}

///////////////////////////////////////////////////////////
Time operator/(int64_t a, Time b) {
    return Time(a / b.microseconds());
}

///////////////////////////////////////////////////////////
Time& operator/=(Time& a, int64_t b) {
    a = Time(a.microseconds() / b);
    return a;
}

///////////////////////////////////////////////////////////
Time operator/(Time a, double b) {
    return Time((int64_t)(a.microseconds() / b));
}

///////////////////////////////////////////////////////////
Time operator/(double a, Time b) {
    return Time((int64_t)(a / b.microseconds()));
}

///////////////////////////////////////////////////////////
Time& operator/=(Time& a, double b) {
    a = Time((int64_t)(a.microseconds() / b));
    return a;
}

///////////////////////////////////////////////////////////
bool operator==(Time a, Time b) {
    return a.microseconds() == b.microseconds();
}

///////////////////////////////////////////////////////////
bool operator!=(Time a, Time b) {
    return a.microseconds() != b.microseconds();
}

///////////////////////////////////////////////////////////
bool operator>(Time a, Time b) {
    return a.microseconds() > b.microseconds();
}

///////////////////////////////////////////////////////////
bool operator<(Time a, Time b) {
    return a.microseconds() < b.microseconds();
}

///////////////////////////////////////////////////////////
bool operator>=(Time a, Time b) {
    return a.microseconds() >= b.microseconds();
}

///////////////////////////////////////////////////////////
bool operator<=(Time a, Time b) {
    return a.microseconds() <= b.microseconds();
}

} // namespace ply