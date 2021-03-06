/* COPYRIGHT (c) 2016-2018 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */

#pragma once

#include <core/os/namespace.hpp>
#include <core/common.hpp>
#include <core/os/Time.hpp>
#include <ch.h>

NAMESPACE_CORE_OS_BEGIN


class Semaphore_:
    private core::Uncopyable
{
public:
    typedef cnt_t Count;

private:
    ::semaphore_t impl;

public:
    void
    initialize(
        Count value = 0
    );

    void
    reset_unsafe(
        Count value = 0
    );

    void
    signal_unsafe();

    void
    wait_unsafe();

    bool
    wait_unsafe(
        const Time& timeout
    );

    void
    reset(
        Count value = 0
    );

    void
    signal();

    void
    wait();

    bool
    wait(
        const Time& timeout
    );


    ::semaphore_t & get_impl();

public:
    Semaphore_(
        Count value = 0
    );
    explicit
    Semaphore_(
        bool  initialize,
        Count value = 0
    );
};


inline
void
Semaphore_::initialize(
    Count value
)
{
    chSemObjectInit(&impl, value);
}

inline
void
Semaphore_::reset_unsafe(
    Count value
)
{
    chSemResetI(&impl, value);
}

inline
void
Semaphore_::signal_unsafe()
{
    chSemSignalI(&impl);
}

inline
void
Semaphore_::wait_unsafe()
{
    chSemWaitS(&impl);
}

inline
bool
Semaphore_::wait_unsafe(
    const Time& timeout
)
{
    return chSemWaitTimeoutS(&impl, timeout.ticks()) == MSG_OK;
}

inline
void
Semaphore_::reset(
    Count value
)
{
    chSemReset(&impl, value);
}

inline
void
Semaphore_::signal()
{
    chSemSignal(&impl);
}

inline
void
Semaphore_::wait()
{
    chSemWait(&impl);
}

inline
bool
Semaphore_::wait(
    const Time& timeout
)
{
    return chSemWaitTimeout(&impl, timeout.ticks()) == MSG_OK;
}

inline
  ::semaphore_t& Semaphore_::get_impl() {
    return impl;
}


inline
Semaphore_::Semaphore_(
    Count value
)
{
    initialize(value);
}

inline
Semaphore_::Semaphore_(
    bool  initialize,
    Count value
)
{
    if (initialize) {
        this->initialize(value);
    }
}

NAMESPACE_CORE_OS_END
