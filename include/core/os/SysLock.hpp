/* COPYRIGHT (c) 2016-2017 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */

#pragma once

#include <core/common.hpp>
#include <core/os/common.hpp>

#include <core/os/impl/SysLock_.hpp>

NAMESPACE_CORE_OS_BEGIN

/*! \brief System lock
 *
 * When the system is in a locked state, the scheduler and the interrupts are disabled.
 */
class SysLock:
    private core::Uncopyable
{
public:
    /*! \brief System lock [RAII]
     *
     * Scoped system lock.
     *
     * \code{.cpp}
     * {
     *   core::os::SysLock::Scope lock(); // acquire the lock
     *
     * } // release lock whenever lock goes out of scope
     * \endcode
     */
    class Scope:
        private core::Uncopyable
    {
    public:
        /*! \brief Acquire the system lock
         *
         */
        Scope()
        {
            SysLock::acquire();
        }

        /*! \brief Release the system lock
         *
         */
        ~Scope()
        {
            SysLock::release();
        }
    };

    /*! \brief System lock [RAII]
     *
     * Scoped system lock - ISR.
     *
     * \code{.cpp}
     * {
     *   core::os::SysLock::ISRScope lock(); // acquire the lock
     *
     * } // release lock whenever lock goes out of scope
     * \endcode
     *
     * \pre Must be used from within an ISR
     *
     */
    class ISRScope:
        private core::Uncopyable
    {
    public:
        /*! \brief Acquire the system lock
         *
         */
        ISRScope()
        {
            SysLock::acquire_from_isr();
        }

        /*! \brief Release the system lock
         *
         */
        ~ISRScope()
        {
            SysLock::release_from_isr();
        }
    };

    /*! \brief System lock [RAII]
     *
     * Scoped system lock - Templated version.
     *
     * \code{.cpp}
     * {
     *   core::os::SysLock::ScopeFrom<CONTEXT> lock(); // acquire the lock
     *
     * } // release lock whenever lock goes out of scope
     * \endcode
     *
     * \tparam CTX calling context
     */
    template <core::os::CallingContext CTX>
    class ScopeFrom:
        private core::Uncopyable
    {};

private:
    SysLock();

public:
    /*! \brief Acquire the system lock
     *
     */
    static void
    acquire();


    /*! \brief Release the system lock
     *
     */
    static void
    release();


    /*! \brief Acquire the system lock
     *
     * \pre Must be called from within an ISR
     *
     */
    static void
    acquire_from_isr();


    /*! \brief Release the system lock
     *
     * \pre Must be called from within an ISR
     *
     */
    static void
    release_from_isr();
};


inline
void
SysLock::acquire()
{
    SysLock_::acquire();
}

inline
void
SysLock::release()
{
    SysLock_::release();
}

inline
void
SysLock::acquire_from_isr()
{
    SysLock_::acquire_from_isr();
}

inline
void
SysLock::release_from_isr()
{
    SysLock_::release_from_isr();
}

template <>
class SysLock::ScopeFrom<core::os::CallingContext::NORMAL> :
    SysLock::Scope
{};

template <>
class SysLock::ScopeFrom<core::os::CallingContext::ISR> :
    SysLock::ISRScope
{};

template <>
class SysLock::ScopeFrom<core::os::CallingContext::SYSLOCK>
{};

NAMESPACE_CORE_OS_END
