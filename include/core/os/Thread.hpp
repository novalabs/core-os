/* COPYRIGHT (c) 2016-2017 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */

#pragma once

#include <core/common.hpp>
#include <core/os/common.hpp>

#include <core/os/MemoryPool.hpp>

#include <core/os/impl/Thread_.hpp>

NAMESPACE_CORE_OS_BEGIN

class Time;

/*! \brief OS Thread abstraction class
 *
 */
class Thread:
    private core::Uncopyable
{
public:
    using Priority = Thread_::Priority; //!< Thread priority type

    /*! \brief Thread function type
     *
     * The thread function signature is \c void(void* args)
     */
    using Function = Thread_::Function;
    using Return   = Thread_::Return;  //!< Thread return and message type
    using Argument = Thread_::Argument; //!< Thread function argument type

    /*! \brief Stack for a statically allocated Thread
     *
     * It adds all the storage required by the OS to the user defined size.
     *
     * \tparam SIZE required (user) stack size
     */
    template <std::size_t SIZE>
    using Stack = Thread_::Stack<SIZE>;

    /*! \brief Thread priorities
     *
     * \warning
     * It is possible to modulate the priority level by adding and subtracting from the constants below.
     * This however is not OS independent and portable.
     */
    enum PriorityEnum : Priority {
        READY     = Thread_::READY,
        IDLE      = Thread_::IDLE, //!< Idle thread, very low priority
        LOWEST    = Thread_::LOWEST, //!< Lowest possible priority
        NORMAL    = Thread_::NORMAL, //!< Normal priority
        HIGHEST   = Thread_::HIGHEST, //!< Highest possible priority
        INTERRUPT = Thread_::INTERRUPT
    };

    enum  {
        OK = Thread_::OK //!< Generic success
    };

public:
    /*! \brief Gets the name of the thread
     *
     * \return a null terminated string with the name of the thread
     * \retval \c nullptr if no name has been specified
     */
    const char*
    get_name() const;


    /*! \brief Sets the name of the thread
     */
    void
    set_name(
        const char* namep //!< [in] a null terminated string with the name of the thread
    );


    /*! \brief Creates a thread into a static memory area
     *
     * \return Pointer to the created thread
     */
    static Thread*
    create_static(
        void*       stackp, //!< [in] pointer to a static memory area that will serve as thread storage and stack
        size_t      stacklen, //!< [in] size of the stack required by the thread
        Priority    priority, //!< [in] priority level of the new thread
        Function    threadf, //!< [in] thread function
        void*       argp, //!< [in] pointer to parameters to be passed to the thread function
        const char* namep = nullptr //!< [in] name of the thread
    );


    /*! \brief Creates a thread into a static memory area
     *
     * \return Pointer to the created thread
     */
    template <std::size_t SIZE>
    static Thread*
    create_static(
        Stack<SIZE>& stack, //!< [in] thread storage and stack
        Priority     priority, //!< [in] priority level of the new thread
        Function     threadf, //!< [in] thread function
        void*        argp, //!< [in] pointer to parameters to be passed to the thread function
        const char*  namep = nullptr //!< [in] name of the thread
    );


    /*! \brief Creates a thread into memory allocated from the heap
     *
     * \return pointer to the created thread
     */
    static Thread*
    create_heap(
        void*       heapp, //!< [in] pointer to heap from which allocate the memory (\c nullptr for the default heap)
        size_t      stacklen, //!< [in] size of the stack required by the thread
        Priority    priority, //!< [in] priority level of the new thread
        Function    threadf, //!< [in] thread function
        void*       argp, //!< [in] pointer to parameters to be passed to the thread function
        const char* namep = nullptr //!< [in] name of the thread
    );


    /*! \brief Creates a thread into memory allocated from the specified memory pool
     *
     * \return pointer to the created thread
     */
    template <typename T>
    static Thread*
    create_pool(
        MemoryPool<T>& mempool, //!< [in] pointer to the memory pool object
        Priority       priority, //!< [in] priority level of the new thread
        Function       threadf, //!< [in] thread function
        void*          argp, //!< [in] pointer to parameters to be passed to the thread function
        const char*    namep = nullptr //!< [in] name of the thread
    );


    /*! \brief Gets a refernce to the current thread
     *
     * \return reference to the current thread
     */
    static Thread&
    self();


    /*! \brief Gets the priority of the current thread
     *
     * \return priority of the current thread
     *
     * \warning
     * It is possible that the returned priority does not match any constants from Thread::PriorityEnum.
     */
    static Priority
    get_priority();


    /*! \brief Sets the priority of the current thread
     *
     * \warning
     * The admissible values depend on the OS.
     * However the values from Thread::PriorityEnum are guaranteed to be ok.
     */
    static void
    set_priority(
        Priority priority //!< [in] priority level of the current thread
    );


    /*! \brief Yields the processor
     *
     */
    static void
    yield();


    /*! \brief Puts the current thread to sleep for a given amount of time
     */
    static void
    sleep(
        const Time& delay //!< [in] how long must it sleep
    );


    /*! \brief Puts the current thread to sleep until a given time
     */
    static void
    sleep_until(
        const Time& time //!< [in] wake up time
    );


    /*! \brief Puts the current thread to sleep until a Thread::wake command
     *
     * \return message value specified in the Thread::wake
     */
    static Return
    sleep();


    /*! \brief Wakes up a thread that has been put to sleep with a Thread::sleep command
     */
    static void
    wake(
        Thread& thread, //!< [in] reference of the thread to wake up
        Return  msg //!< [in] message to pass the sleeping thread (it will be returned by Thread::sleep)
    );


    /*! \brief Exits the current thread.
     */
    static void
    exit(
        uint32_t msg //!< [in] thread exit code
    );


    /*! \brief Waits for the specified thread to finish.
     *
     * \return Success
     * \retval true the thread exit code was OK
     */
    static bool
    join(
        Thread& thread //!< [in] thread to wait for
    );


    /*! \brief Sets a "terminate request" flag for the thread
     *
     * The flag should be periodically checked using Thread::should_terminate by the thread function.
     */
    static void
    terminate(
        Thread& thread //!< [in] the to-be-terminated thread
    );


    /*! \brief Check if the thread must be terminated
     *
     *  \return the terminate request flag
     *  \retval true Thread::terminate has been called for the current thread
     */
    static bool
    should_terminate();


    /*! \brief Thread equivalence
     *
     *  \return Thread equivalence
     *  \retval true if this == other
     */
    bool
    operator==(
        const Thread& other //!< [in] other thread
    );


    /*! \brief Thread inequality
     *
     *  \return Thread inequality
     *  \retval true if this != other
     */
    bool
    operator!=(
        const Thread& other //!< [in] other thread
    );


private:
    Thread();

    Thread_ impl;

    static size_t
    compute_stack_size(
        size_t userlen
    );
};

/* ------------------------------------------------------------------------- */

inline
const char*
Thread::get_name() const
{
    return impl.get_name();
}

inline
void
Thread::set_name(
    const char* namep
)
{
    impl.set_name(namep);
}

inline
size_t
Thread::compute_stack_size(
    size_t userlen
)
{
    return Thread_::compute_stack_size(userlen);
}

inline
Thread*
Thread::create_static(
    void*       stackp,
    size_t      stacklen,
    Priority    priority,
    Function    threadf,
    void*       argp,
    const char* namep
)
{
    return reinterpret_cast<Thread*>(
        Thread_::create_static(stackp, stacklen, priority, threadf, argp, namep)
    );
}

template <std::size_t SIZE>
inline Thread*
Thread::create_static(
    Stack<SIZE>& stack,
    Priority     priority,
    Function     threadf,
    void*        argp,
    const char*  namep
)
{
    return reinterpret_cast<Thread*>(
        Thread_::create_static(stack, stack.size(), priority, threadf, argp, namep)
    );
}

inline
Thread*
Thread::create_heap(
    void*       heapp,
    size_t      stacklen,
    Priority    priority,
    Function    threadf,
    void*       argp,
    const char* namep
)
{
    return reinterpret_cast<Thread*>(
        Thread_::create_heap(heapp, stacklen, priority, threadf, argp, namep)
    );
}

template <typename T>
inline
Thread*
Thread::create_pool(
    MemoryPool<T>& mempool,
    Priority       priority,
    Function       threadf,
    void*          argp,
    const char*    namep
)
{
    return reinterpret_cast<Thread*>(
        Thread_::create_pool(mempool, priority, threadf, argp, namep)
    );
}

inline
Thread&
Thread::self()
{
    return reinterpret_cast<Thread&>(Thread_::self());
}

inline
Thread::Priority
Thread::get_priority()
{
    return Thread_::get_priority();
}

inline
void
Thread::set_priority(
    Priority priority
)
{
    Thread_::set_priority(priority);
}

inline
void
Thread::yield()
{
    Thread_::yield();
}

inline
void
Thread::sleep(
    const Time& delay
)
{
    Thread_::sleep(delay);
}

inline
void
Thread::sleep_until(
    const Time& time
)
{
    Thread_::sleep_until(time);
}

inline
Thread::Return
Thread::sleep()
{
    return Thread_::sleep();
}

inline void
Thread::wake(
    Thread&        thread,
    Thread::Return msg
)
{
    Thread_::wake(thread.impl, msg);
}

inline
void
Thread::exit(
    uint32_t msg
)
{
    Thread_::exit(msg);
}

inline
bool
Thread::join(
    Thread& thread
)
{
    return Thread_::join(thread.impl);
}

inline
void
Thread::terminate(
    Thread& thread
)
{
    Thread_::terminate(thread.impl);
}

inline bool
Thread::should_terminate()
{
    return Thread_::should_terminate();
}

inline bool
Thread::operator==(
    const Thread& other
)
{
    return impl == other.impl;
}

inline bool
Thread::operator!=(
    const Thread& other
)
{
    return impl != other.impl;
}

NAMESPACE_CORE_OS_END
