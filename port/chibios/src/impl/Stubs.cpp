/* COPYRIGHT (c) 2016-2017 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */

#ifndef SKIP_CORE_STUBS

#include "osal.h"

// disable name demangling
namespace __gnu_cxx {
void
__verbose_terminate_handler()
{
    osalSysHalt("__verbose_terminate_handler.");
}
}

namespace std {
void
__throw_bad_function_call()
{
    osalSysHalt("__throw_bad_function_call");
}
}

typedef int __guard;

extern "C" {
#include <stdio.h>
#include <errno.h>

#include "osal.h"

    int
    __cxa_guard_acquire(
        __guard* g
    )
    {
        return !*(char*)(g);
    }

    void
    __cxa_guard_release(
        __guard* g
    )
    {
        *(char*)g = 1;
    }

    void
    __cxa_guard_abort(
        __guard* g __attribute__((unused))
    ) {}

    void
    _exit(
        int status
    )
    {
        (void)status;
        osalSysHalt("Unrealized");
    }

    pid_t
    _getpid(
        void
    )
    {
        return 1;
    }

#undef errno
    extern int errno;
    int
    _kill(
        int pid,
        int sig
    )
    {
        (void)pid;
        (void)sig;
        errno = EINVAL;
        return -1;
    }

    void
    _open_r(
        void
    )
    {
        return;
    }

    void
    __cxa_pure_virtual()
    {
        osalSysHalt("Pure virtual function call.");
    }
}

#include <ch.h>


void*
operator new(
    size_t size
)
{
    return chHeapAlloc(NULL, (size > 0) ? size : 1);
}

void*
operator new(
    size_t           size,
      ::memory_heap* heapp
)
{
    return chHeapAlloc(heapp, (size > 0) ? size : 1);
}

void*
operator new [](
    size_t size
)
{
    return chHeapAlloc(NULL, (size > 0) ? size : 1);
}

void*
operator new [](
    size_t           size,
      ::memory_heap* heapp
)
{
    return chHeapAlloc(heapp, (size > 0) ? size : 1);
}

void
operator delete(
    void* objp
)
{
    if (objp != NULL) {
        chHeapFree(objp);
    }
}

void
operator delete(
    void*            objp,
      ::memory_heap* heapp
)
{
    (void)heapp;

    if (objp != NULL) {
        chHeapFree(objp);
    }
}

void
operator delete [](
    void* objp
)
{
    if (objp != NULL) {
        chHeapFree(objp);
    }
}

void
operator delete [](
    void*            objp,
      ::memory_heap* heapp
)
{
    (void)heapp;

    if (objp != NULL) {
        chHeapFree(objp);
    }
}

#endif // ifndef SKIP_CORE_STUBS
