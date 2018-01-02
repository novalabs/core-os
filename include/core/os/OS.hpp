/* COPYRIGHT (c) 2016-2018 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */

#pragma once

#include <core/common.hpp>
#include <core/os/common.hpp>

#include <core/os/impl/OS_.hpp>

NAMESPACE_CORE_OS_BEGIN

class OS:
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
    OS();
};


inline
void
OS::initialize()
{
    OS_::initialize();
}

inline void
OS::halt(
    const char* message
)
{
    OS_::halt(message);
}

inline void
OS::enable()
{
    OS_::enable();
}

inline void
OS::disable()
{
    OS_::disable();
}

NAMESPACE_CORE_OS_END
