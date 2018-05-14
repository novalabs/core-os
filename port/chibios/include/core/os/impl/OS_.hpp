/* COPYRIGHT (c) 2016-2018 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */

#pragma once

#include <core/os/namespace.hpp>
#include <core/common.hpp>
#include <hal.h>
#include <osal.h>

NAMESPACE_CORE_OS_BEGIN


class OS_:
    private core::Uncopyable
{
public:
    static void
    initialize();

    static void
    halt(
        const char* message
    );

    static void
    enable();

    static void
    disable();


private:
    OS_();
};


inline
void
OS_::initialize()
{
    halInit();
    chSysInit();
}

inline void
OS_::halt(
    const char* message
)
{
    osalSysHalt(message);
}

inline void
OS_::enable()
{
    osalSysEnable();
}

inline void
OS_::disable()
{
    osalSysDisable();
}

NAMESPACE_CORE_OS_END
