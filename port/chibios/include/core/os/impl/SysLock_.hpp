/* COPYRIGHT (c) 2016-2018 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */

#pragma once

#include <core/os/namespace.hpp>
#include <core/common.hpp>
#include <osal.h>

NAMESPACE_CORE_OS_BEGIN


class SysLock_:
    private core::Uncopyable
{
private:
    SysLock_();

public:
    static void
    acquire();

    static void
    release();

    static void
    acquire_from_isr();

    static void
    release_from_isr();
};


inline
void
SysLock_::acquire()
{
    osalSysLock();
}

inline
void
SysLock_::release()
{
    osalOsRescheduleS();
    osalSysUnlock();
}

inline
void
SysLock_::acquire_from_isr()
{
    osalSysLockFromISR();
}

inline
void
SysLock_::release_from_isr()
{
    osalSysUnlockFromISR();
}

NAMESPACE_CORE_OS_END
