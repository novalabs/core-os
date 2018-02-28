/* COPYRIGHT (c) 2016-2018 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */

#include <core/os/namespace.hpp>
#include <core/os/Time.hpp>
#include <osal.h>
#include <limits>

NAMESPACE_CORE_OS_BEGIN

static_assert(CH_CFG_ST_FREQUENCY == 1000, "CH_CFG_ST_FREQUENCY must be 1000. I am sorry about that.");

Time
Time::now()
{
    register Type t = osalOsGetSystemTimeX();

    return ticks(t);
}

NAMESPACE_CORE_OS_END
