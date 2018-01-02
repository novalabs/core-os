/* COPYRIGHT (c) 2016-2018 Nova Labs SRL
 *
 * All rights reserved. All use of this software and documentation is
 * subject to the License Agreement located in the file LICENSE.
 */

#pragma once

#include <core/common.hpp>
#include <core/os/common.hpp>
#include <core/os/Mutex.hpp>

NAMESPACE_CORE_OS_BEGIN

#ifndef CORE_BLOCK_ALLOCATOR_STATS
#define CORE_BLOCK_ALLOCATOR_STATS false
#endif

/*! \brief A pretty stupid block allocator.
 *
 * \warning This is WIP - and so it is subject to changes
 */
class BlockAllocator
{
public:
    BlockAllocator(
        void*       storage,
        std::size_t size
    ) : _size(size), _free(size), _root(nullptr), _storage(nullptr)
    {
        _lock.initialize();
        initialize(storage, size);
    }

    BlockAllocator() : _size(0), _free(0), _root(nullptr), _storage(nullptr)
    {
        _lock.initialize();
#if CORE_BLOCK_ALLOCATOR_STATS
        _blocks = 0;
        _used   = 0;
#endif
    }

    template <core::os::CallingContext CTX = core::os::CallingContext::NORMAL>
    void
    initialize(
        void*       storage,
        std::size_t size
    )
    {
        lock<CTX>();
        _storage = reinterpret_cast<uint8_t*>(storage);
        CORE_ASSERT(_storage != nullptr);

        _size = size;
        _free = size;

#if CORE_BLOCK_ALLOCATOR_STATS
        _blocks = 0;
        _used   = 0;
#endif
        unlock<CTX>();
    }

    template <core::os::CallingContext CTX = core::os::CallingContext::NORMAL>
    void*
    alloc(
        std::size_t size,
        uint8_t     refcount = 1
    )
    {
        CORE_ASSERT(refcount > 0);

        lock<CTX>();

        Item* item = _root;

        while (item != nullptr) {
            if ((item->flags == 0) && (item->size == size)) {
                item->flags    = 1;
                item->refcount = refcount;
#if CORE_BLOCK_ALLOCATOR_STATS
                _used++;
#endif
                unlock<CTX>();
                return item->data;
            }

            item = item->next;
        }

        item = (Item*)getBlock(size);

        if (item != nullptr) {
            item->size     = size;
            item->refcount = refcount;
            item->flags    = 1;
            item->next     = _root;
            item->data     = reinterpret_cast<void*>((uint8_t*)item + sizeof(Item));
            _root = item;
#if CORE_BLOCK_ALLOCATOR_STATS
            _blocks++;
            _used++;
#endif
            unlock<CTX>();
            return item->data;
        }

        unlock<CTX>();
        return nullptr;
    } // alloc

    template <core::os::CallingContext CTX = core::os::CallingContext::NORMAL>
    void
    clean()
    {
        lock<CTX>();
        Item* item = _root;

        while (item != nullptr) {
            if (item->refcount == 0) {
                item->flags = 0;
#if CORE_BLOCK_ALLOCATOR_STATS
                _used--;
#endif
            }

            item = item->next;
        }

        unlock<CTX>();
    } // clean

    template <core::os::CallingContext CTX = core::os::CallingContext::NORMAL>
    void*
    use(
        void* data
    )
    {
        lock<CTX>();

        if (data != nullptr) {
            Item* item = reinterpret_cast<Item*>(reinterpret_cast<uint8_t*>(data) - sizeof(Item));

            if (item->data == data) {
                if ((item->flags == 1) && (item->refcount > 0)) {
                    item->refcount++;

                    unlock<CTX>();
                    return data;
                }
            }
        }

        unlock<CTX>();
        return nullptr;
    } // use

    template <core::os::CallingContext CTX = core::os::CallingContext::NORMAL>
    void*
    free(
        void* data
    )
    {
        lock<CTX>();

        if (data != nullptr) {
            Item* item = reinterpret_cast<Item*>(reinterpret_cast<uint8_t*>(data) - sizeof(Item));

            if ((item->data == data) && (item->flags == 1)) {
                item->refcount--;

                if (item->refcount == 0) {
                    item->flags = 0;
#if CORE_BLOCK_ALLOCATOR_STATS
                    _used--;
#endif
                    unlock<CTX>();
                    return nullptr;
                } else if (item->refcount < 0) {
                    CORE_ASSERT(!"Double free");
                }
            }
        }

        unlock<CTX>();
        return data;
    } // free

    template <core::os::CallingContext CTX = core::os::CallingContext::NORMAL>
    void*
    robust_free(
        void* data
    )
    {
        lock<CTX>();
        Item* item = _root;

        if (data != nullptr) {
            while (item != nullptr) {
                if ((item->data == data) && (item->refcount == 0) && (item->flags == 1)) {
                    item->flags = 0;
#if CORE_BLOCK_ALLOCATOR_STATS
                    _used--;
#endif
                    unlock<CTX>();
                    return nullptr;
                }

                item = item->next;
            }
        }

        unlock<CTX>();
        return data;
    } // free

private:
    struct Item {
        uint16_t size;
        int8_t   refcount;
        uint8_t  flags;
        Item*    next;
        void*    data;
    } CORE_PACKED; // Aligned by design

    void*
    getBlock(
        std::size_t size
    )
    {
        CORE_ASSERT(_storage != nullptr);

        std::size_t alignedSize = sizeof(Item) + size;

        if ((alignedSize % 4) != 0) {
            alignedSize += 4 - (alignedSize % 4);
        }

        if (_free < alignedSize) {
            return nullptr;
        }

        void* tmp = _storage;
        _free    -= alignedSize;
        _storage += alignedSize;

        return tmp;
    } // getBlock

    std::size_t     _size;
    std::size_t     _free;
    Item*           _root;
    uint8_t*        _storage;
    core::os::Mutex _lock;
#if CORE_BLOCK_ALLOCATOR_STATS
    uint32_t _blocks;
    uint32_t _used;
#endif

    template <core::os::CallingContext CTX>
    inline void
    lock();


    template <core::os::CallingContext CTX>
    inline void
    unlock();
};

template <>
inline void
BlockAllocator::lock<core::os::CallingContext::NORMAL>()
{
    _lock.acquire();
}

template <>
inline void
BlockAllocator::lock<core::os::CallingContext::SYSLOCK>()
{
    _lock.acquire_unsafe();
}

template <>
inline void
BlockAllocator::unlock<core::os::CallingContext::NORMAL>()
{
    _lock.release();
}

template <>
inline void
BlockAllocator::unlock<core::os::CallingContext::SYSLOCK>()
{
    _lock.release_unsafe();
}

NAMESPACE_CORE_OS_END
