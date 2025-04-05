#pragma once

#include <cstdint>

namespace ply
{

///////////////////////////////////////////////////////////
/// \brief A class for handling conversions between different time units
///
///////////////////////////////////////////////////////////
class Time
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor that initializes with zero (microseconds)
	///
	///////////////////////////////////////////////////////////
	Time();

	///////////////////////////////////////////////////////////
	/// \brief Construct time object using microseconds
	///
	/// \see fromMicroseconds
	///
	///////////////////////////////////////////////////////////
	Time(int64_t microseconds);

	///////////////////////////////////////////////////////////
	/// \brief Static function that constructs a time object from minutes
	///
	/// \param minutes Amount of minutes
	///
	/// \return A time object
	///
	///////////////////////////////////////////////////////////
	static Time fromMinutes(double minutes);

	///////////////////////////////////////////////////////////
	/// \brief Static function that constructs a time object from seconds
	///
	/// \param seconds Amount of seconds
	///
	/// \return A time object
	///
	/// \see fromMilliseconds, fromMicroseconds
	///
	///////////////////////////////////////////////////////////
	static Time fromSeconds(float seconds);

	///////////////////////////////////////////////////////////
	/// \brief Static function that constructs a time object from milliseconds
	///
	/// \param milliseconds Amount of milliseconds
	///
	/// \return A time object
	///
	/// \see fromSeconds, fromMicroseconds
	///
	///////////////////////////////////////////////////////////
	static Time fromMilliseconds(int32_t milliseconds);

	///////////////////////////////////////////////////////////
	/// \brief Static function that constructs a time object from microseconds
	///
	/// \param microseconds Amount of microseconds
	///
	/// \return A time object
	///
	/// \see fromSeconds, fromMilliseconds
	///
	///////////////////////////////////////////////////////////
	static Time fromMicroseconds(int64_t microseconds);

	///////////////////////////////////////////////////////////
	/// \brief Get the stored time in minutes
	///
	/// \return Double representing time in minutes
	///
	///////////////////////////////////////////////////////////
	double toMinutes() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the stored time in seconds
	///
	/// \return Float representing time in seconds
	///
	/// \see toMilliseconds, toMicroseconds
	///
	///////////////////////////////////////////////////////////
	float toSeconds() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the stored time in milliseconds
	///
	/// \return Int32 representing time in milliseconds
	///
	/// \see toSeconds, toMicroseconds
	///
	///////////////////////////////////////////////////////////
	int32_t toMilliseconds() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the stored time in microseconds
	///
	/// \return Int64 representing time in microseconds
	///
	/// \see toSeconds, toMilliseconds
	///
	///////////////////////////////////////////////////////////
	int64_t toMicroseconds() const;

private:
	int64_t m_microseconds; //!< The time in microseconds
};

Time operator-(Time a, Time b);
Time operator-=(Time& a, Time b);
Time operator+(Time a, Time b);
Time operator+=(Time& a, Time b);

Time operator*(Time a, int64_t b);
Time operator*(int64_t a, Time b);
Time operator*(Time a, double b);
Time operator*(double a, Time b);
Time& operator*=(Time& a, int64_t b);
Time& operator*=(Time& a, double b);

Time operator/(Time a, int64_t b);
Time operator/(int64_t a, Time b);
Time operator/(Time a, double b);
Time operator/(double a, Time b);
Time& operator/=(Time& a, int64_t b);
Time& operator/=(Time& a, double b);

bool operator==(Time a, Time b);
bool operator!=(Time a, Time b);
bool operator>(Time a, Time b);
bool operator<(Time a, Time b);
bool operator>=(Time a, Time b);
bool operator<=(Time a, Time b);

} // namespace ply

///////////////////////////////////////////////////////////
/// \class poly::Time
/// \ingroup Core
///
/// poly::Time is a class that handles conversions to and
/// from seconds, milliseconds, and microseconds. To create
/// a time object, use one of the from*() functions to convert
/// from a certain unit. To get the time in a certain unit,
/// use one of the to*() functions to convert to that unit.
///
/// Each unit is represented with a different data type:
/// \li float: Seconds
/// \li Int32: Milliseconds
/// \li Int64: Microseconds
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // Store time in milliseconds and convert to seconds
/// Time t = Time::fromMilliseconds(500);
/// float sec = t.toSeconds(); // 0.5f
///
/// // Store time in microseconds and convert to milliseconds
/// t = Time::fromMicroseconds(1051);
/// Int32 milli = t.toMilliseconds(); // 1 because conversions will truncate for integers
///
/// \endcode
///
///////////////////////////////////////////////////////////