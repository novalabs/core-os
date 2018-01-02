/* COPYRIGHT (c) 2016-2018 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */

#pragma once

#include <core/os/namespace.hpp>

NAMESPACE_CORE_OS_BEGIN

/*! \brief CallingContext
 *
 * \todo This has to be explained quite well
 */
enum class CallingContext {
    NORMAL, //!< Normal (thread)
    ISR, //!< ISR (interrupt service routine)
    SYSLOCK //!< Syslocked
};

NAMESPACE_CORE_OS_END
