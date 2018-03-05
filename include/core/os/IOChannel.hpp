/* COPYRIGHT (c) 2016-2018 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */

#pragma once

#ifndef IOCHANNEL_USE_CHPRINTF
#ifdef CHIBIOS_HAS_CHPRINTF
#define IOCHANNEL_USE_CHPRINTF TRUE
#else
#define IOCHANNEL_USE_CHPRINTF FALSE
#endif
#else
#if IOCHANNEL_USE_CHPRINTF && not defined(CHIBIOS_HAS_CHPRINTF)
#error IOCHANNEL_USE_CHPRINTF is true but no there is no chprintf
#endif
#endif

#include <core/common.hpp>
#include <core/os/common.hpp>

#include <core/os/Time.hpp>

#if IOCHANNEL_USE_CHPRINTF
#include <chprintf.h>
#endif

#include <hal_channels.h>
#include <stdarg.h>

NAMESPACE_CORE_OS_BEGIN

/*! \brief Base class for all IO hal_channels
 *
 */
class IOChannel
{
public:
    using Channel = BaseChannel *;

    enum class DefaultTimeout {
        INFINITE, IMMEDIATE
    };

    /*! \brief Put a byte in the channel
     *
     * \return number of bytes put
     */
    virtual std::size_t
    put(
        const uint8_t& x, //!< [in] byte
        core::os::Time timeout //!< [in] timeout
    ) = 0;


    /*! \brief Get a byte from the channel
     *
     * \return number of bytes get
     */
    virtual std::size_t
    get(
        uint8_t&       x, //!< [out] byte
        core::os::Time timeout //!< [in] timeout
    ) = 0;


    /*! \brief Write data to a channel
     *
     * \return number of bytes written
     */
    virtual std::size_t
    write(
        const uint8_t* buffer, //!< [in] data buffer
        std::size_t    size, //!< [in] size of the data (<= size of the data buffer)
        core::os::Time timeout //!< [in] timeout
    ) = 0;


    /*! \brief Read data from a channel
     *
     * \return number of bytes read
     */
    virtual std::size_t
    read(
        uint8_t*       buffer, //!< [in] data buffer
        std::size_t    size, //!< [in] size of the data (<= size of the data buffer)
        core::os::Time timeout //!< [in] timeout
    ) = 0;


#if IOCHANNEL_USE_CHPRINTF || defined(__DOXYGEN__)
    /*! \brief Formatted print to a channel
     *
     * \warning Only a subset of the "full" format parameters is supported.
     *
     * \return number of bytes written
     */
    virtual int
    printf(
        const char* fmt, //!< [in] formatting string
        ...
    ) = 0;
#endif

    virtual
    Channel
    rawChannel() = 0;
};

template <class _SD>
struct SDChannelTraits {
    static constexpr auto channel = (IOChannel::Channel)_SD::driver;
};


template <class _CHANNEL, IOChannel::DefaultTimeout _DEFAULT_TIMEOUT>
class IOChannel_:
    public IOChannel
{
public:
    using Channel = _CHANNEL;
    static constexpr core::os::Time
    DEFAULT_TIMEOUT()
    {
        return _DEFAULT_TIMEOUT == IOChannel::DefaultTimeout::INFINITE ? core::os::Time::INFINITE : core::os::Time::IMMEDIATE;
    }

    inline std::size_t
    put(
        const uint8_t& x,
        core::os::Time timeout = DEFAULT_TIMEOUT()
    )
    {
        return (chnPutTimeout(Channel::channel, x, timeout.ticks()) == Q_OK) ? 1 : 0;
    }

    inline std::size_t
    get(
        uint8_t&       x,
        core::os::Time timeout = DEFAULT_TIMEOUT()
    )
    {
        msg_t tmp = chnGetTimeout(Channel::channel, timeout.ticks());

        if ((tmp & 0xFFFFFF00) == 0) {
            x = (uint8_t)(tmp & 0x000000FF);
            return 1;
        } else {
            return 0;
        }
    }

    inline std::size_t
    write(
        const uint8_t* buffer,
        std::size_t    size,
        core::os::Time timeout = DEFAULT_TIMEOUT()
    )
    {
        return chnWriteTimeout(Channel::channel, buffer, size, timeout.ticks());
    }

    inline std::size_t
    read(
        uint8_t*       buffer,
        std::size_t    size,
        core::os::Time timeout = DEFAULT_TIMEOUT()
    )
    {
        return chnReadTimeout(Channel::channel, buffer, size, timeout.ticks());
    }

#if IOCHANNEL_USE_CHPRINTF
    inline int
    printf(
        const char* fmt,
        ...
    )
    {
        va_list ap;
        int     formatted_bytes;

        va_start(ap, fmt);
        formatted_bytes = chvprintf(reinterpret_cast<BaseSequentialStream*>(rawChannel()), fmt, ap);
        va_end(ap);

        return formatted_bytes;

    }
#endif

    inline IOChannel::Channel
    rawChannel()
    {
        return Channel::channel;
    }
};

NAMESPACE_CORE_OS_END
