/* COPYRIGHT (c) 2016 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */

#pragma once

#include <core/os/namespace.hpp>
#include <core/common.hpp>
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

NAMESPACE_CORE_OS_END
