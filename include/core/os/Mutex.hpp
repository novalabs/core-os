/* COPYRIGHT (c) 2016-2017 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */

#pragma once

#include <core/os/namespace.hpp>
#include <core/common.hpp>
#include <core/os/impl/Mutex_.hpp>

NAMESPACE_CORE_OS_BEGIN

/*! \brief Mutex
 *
 */
class Mutex:
    private core::Uncopyable
{
private:
    Mutex_ impl;

public:
    /*! \brief Initialize the mutex
     *
     */
    void
    initialize();


    /*! \brief Acquire ownership
     *
     * If the mutex is already owned, the requesting thread is put to sleep and queued.
     *
     * \warning Must be used only in a system lock zone by threads only.
     */
    void
    acquire_unsafe();


    /*! \brief Relinquishes ownership
     *
     * \pre The invoking thread must have the ownership of the mutex
     *
     * \warning Must be used only in a system lock zone by threads only.
     */
    void
    release_unsafe();


    /*! \brief Acquire ownership
     *
     * If the mutex is already owned, the requesting thread is put to sleep and queued.
     *
     * \warning Must be used only outside a system lock zone.
     */
    void
    acquire();


    /*! \brief Relinquishes ownership
     *
     * \pre The invoking thread must have the ownership of the mutex
     *
     * \warning Must be used only outside a system lock zone.
     */
    void
    release();


public:
    Mutex();
    explicit
    Mutex(
        bool initialize
    );
};


inline
void
Mutex::initialize()
{
    impl.initialize();
}

inline
void
Mutex::acquire_unsafe()
{
    impl.acquire_unsafe();
}

inline
void
Mutex::release_unsafe()
{
    impl.release_unsafe();
}

inline
void
Mutex::acquire()
{
    impl.acquire();
}

inline
void
Mutex::release()
{
    impl.release();
}

inline
Mutex::Mutex()
    :
    impl()
{}


inline
Mutex::Mutex(
    bool initialize
)
    :
    impl(initialize)
{}


NAMESPACE_CORE_OS_END
