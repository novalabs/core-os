/* COPYRIGHT (c) 2016-2017 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */

#pragma once

#include <core/os/namespace.hpp>
#include <core/common.hpp>
#include <osal.h>

NAMESPACE_CORE_OS_BEGIN


class OS_:
    private core::Uncopyable
{
public:
    static void
    initialize();

    static const void
    halt(
        const char* message
    );


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

inline const void
OS_::halt(
    const char* message
)
{
    osalSysHalt(message);
}

NAMESPACE_CORE_OS_END
