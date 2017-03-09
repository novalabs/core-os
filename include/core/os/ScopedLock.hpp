/* COPYRIGHT (c) 2016-2017 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */

#pragma once

#include <core/common.hpp>
#include <core/os/common.hpp>

NAMESPACE_CORE_OS_BEGIN


template <typename Lockable>
class ScopedLock:
    private core::Uncopyable
{
private:
    Lockable& lock;

public:
    ScopedLock(
        Lockable& lock
    );
    ~ScopedLock();
};


template <typename Lockable>
inline
ScopedLock<Lockable>::ScopedLock(
    Lockable& lock
)
    :
    lock(lock)
{
    this->lock.acquire();
}

template <typename Lockable>
inline
ScopedLock<Lockable>::~ScopedLock()
{
    this->lock.release();
}

NAMESPACE_CORE_OS_END
