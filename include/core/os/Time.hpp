/* COPYRIGHT (c) 2016-2018 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */

#pragma once

#include <core/common.hpp>
#include <core/os/common.hpp>

#include <limits>

NAMESPACE_CORE_OS_BEGIN

/*! \brief Time intervals
 *
 */
class Time
{
public:
    using Type = uint32_t;

public:
    /*! \brief Get the time in system ticks
     *
     */
    uint32_t
    ticks() const;


    /*! \brief Get the time in whole ms
     *
     */
    Type
    ms() const;


    /*! \brief Get the time in whole s
     *
     */
    Type
    s() const;


    /*! \brief Get the time in ms
     *
     */
    float
    to_ms() const;


    /*! \brief Get the time in s
     *
     */
    float
    to_s() const;

    /*! \brief Assignement
     *
     */
    Time&
    operator=(
        const Time& rhs //!< [in] interval to be assigned
    );


    /*! \brief Increment the time
     *
     */
    Time&
    operator+=(
        const Time& rhs //!< [in] interval to be added
    );


    /*! \brief Decrement the time
     *
     */
    Time&
    operator-=(
        const Time& rhs //!< [in] interval to be subtracted
    );


public:
    Time();

    template <typename T>
    Time(
        T milliseconds
    );

    explicit
    Time(
        float seconds
    );

    Time(
        const Time& rhs
    );

public:
    /*! \brief Returns a time interval
     *
     */
    static Time
    ticks(
        const Type ticks //!< [in] inteval in ticks
    );

    /*! \brief Returns a time interval
     *
     */
    static Time
    ms(
        const Type milliseconds //!< [in] inteval in ms
    );


    /*! \brief Returns a time interval
     *
     */
    static Time
    s(
        const Type seconds //!< [in] inteval in s
    );


    /*! \brief Returns the actual time
     *
     */
    static Time
    now();


public:
    static const Time IMMEDIATE; //!< A null time interval
    static const Time INFINITE; //!< An infinite time interval

public:
    Type raw;
};

bool
operator==(
    const Time& lhs,
    const Time& rhs
);

bool
operator!=(
    const Time& lhs,
    const Time& rhs
);

bool
operator>(
    const Time& lhs,
    const Time& rhs
);

bool
operator>=(
    const Time& lhs,
    const Time& rhs
);

bool
operator<(
    const Time& lhs,
    const Time& rhs
);

bool
operator<=(
    const Time& lhs,
    const Time& rhs
);

const Time
operator+(
    const Time& lhs,
    const Time& rhs
);

const Time
operator-(
    const Time& lhs,
    const Time& rhs
);

inline
uint32_t
Time::ticks() const
{
    return raw;

}

inline
Time::Type
Time::ms() const
{
    return raw;
}

inline
Time::Type
Time::s() const
{
    return raw / 1000;
}

inline
float
Time::to_ms() const
{
    return raw;
}

inline
float
Time::to_s() const
{
    return raw / 1000.0f;
}

inline
Time&
Time::operator=(
    const Time& rhs
)
{
    raw = rhs.raw;
    return *this;
}

inline
Time&
Time::operator+=(
    const Time& rhs
)
{
    raw += rhs.raw;
    return *this;
}

inline
Time&
Time::operator-=(
    const Time& rhs
)
{
    raw -= rhs.raw;
    return *this;
}

inline
Time::Time() : raw() {}


template <typename T>
inline
Time::Time(
    T milliseconds
)
    :
    raw(static_cast<Type>(milliseconds))
{}

template <>
inline
Time::Time(
    systime_t milliseconds
)
    :
    raw(milliseconds)
{}

inline
Time::Time(
    float seconds
)
    :
    raw(static_cast<Type>((seconds + 0.5) / 1000.0))
{}


inline
Time::Time(
    const Time& rhs
) : raw(rhs.raw) {}

inline
Time
Time::ticks(
    const Type ticks
)
{
    return Time(ticks);
}

inline
Time
Time::ms(
    const Type milliseconds
)
{
    return Time(milliseconds);
}

inline
Time
Time::s(
    const Type seconds
)
{
    return Time(seconds * 1000);
}

inline
bool
operator==(
    const Time& lhs,
    const Time& rhs
)
{
    return lhs.raw == rhs.raw;
}

inline
bool
operator!=(
    const Time& lhs,
    const Time& rhs
)
{
    return lhs.raw != rhs.raw;
}

inline
bool
operator>(
    const Time& lhs,
    const Time& rhs
)
{
    return lhs.raw > rhs.raw;
}

inline
bool
operator>=(
    const Time& lhs,
    const Time& rhs
)
{
    return lhs.raw >= rhs.raw;
}

inline
bool
operator<(
    const Time& lhs,
    const Time& rhs
)
{
    return lhs.raw < rhs.raw;
}

inline
bool
operator<=(
    const Time& lhs,
    const Time& rhs
)
{
    return lhs.raw <= rhs.raw;
}

inline
const Time
operator+(
    const Time& lhs,
    const Time& rhs
)
{
    return Time(lhs.raw + rhs.raw);
}

inline
const Time
operator-(
    const Time& lhs,
    const Time& rhs
)
{
    return Time(lhs.raw - rhs.raw);
}

NAMESPACE_CORE_OS_END
