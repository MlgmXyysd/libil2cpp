#pragma once

#include <stdint.h>
#include "utils/NonCopyable.h"
#include "c-api/Atomic-c-api.h"

namespace il2cpp
{
namespace os
{
    class Atomic : public il2cpp::utils::NonCopyable
    {
    public:
        // All 32bit atomics must be performed on 4-byte aligned addresses. All 64bit atomics must be
        // performed on 8-byte aligned addresses.

        // Add and Add64 return the *result* of the addition, not the old value! (i.e. they work like
        // InterlockedAdd and __sync_add_and_fetch).

        static inline void FullMemoryBarrier()
        {
            UnityPalFullMemoryBarrier();
        }

        static inline int32_t Add(int32_t* location1, int32_t value)
        {
            return UnityPalAdd(location1, value);
        }

        static inline uint32_t Add(uint32_t* location1, uint32_t value)
        {
            return (uint32_t)Add((int32_t*)location1, (int32_t)value);
        }

#if IL2CPP_ENABLE_INTERLOCKED_64_REQUIRED_ALIGNMENT
        static inline int64_t Add64(int64_t* location1, int64_t value)
        {
            return UnityPalAdd64(location1, value);
        }

#endif

        template<typename T>
        static inline T* CompareExchangePointer(T** dest, T* newValue, T* oldValue)
        {
            return static_cast<T*>(UnityPalCompareExchangePointer((void**)dest, newValue, oldValue));
        }

        template<typename T>
        static inline T* ExchangePointer(T** dest, const T* newValue)
        {
            return (T*)Baselib_atomic_exchange_ptr_seq_cst((intptr_t*)dest, (intptr_t)newValue);
        }

        static inline int64_t Read64(int64_t* addr)
        {
            return UnityPalRead64(addr);
        }

        static inline uint64_t Read64(uint64_t* addr)
        {
            return (uint64_t)Read64((int64_t*)addr);
        }

        static inline int32_t LoadRelaxed(const int32_t* addr)
        {
            return UnityPalLoadRelaxed(addr);
        }

        template<typename T>
        static inline T* LoadPointerRelaxed(const T* const * addr)
        {
            return (T*)Baselib_atomic_load_ptr_relaxed((const intptr_t*)addr);
        }

        template<typename T>
        static inline T* ReadPointer(T** pointer)
        {
            return (T*)Baselib_atomic_load_ptr_relaxed((intptr_t*)pointer);
        }

        template<typename T>
        static inline void PublishPointer(T** pointer, T* value)
        {
            Baselib_atomic_thread_fence_release();
            Baselib_atomic_store_ptr_relaxed((intptr_t*)pointer, (intptr_t)value);
        }

        static inline int32_t Increment(int32_t* value)
        {
            return UnityPalIncrement(value);
        }

        static inline uint32_t Increment(uint32_t* value)
        {
            return (uint32_t)Increment((int32_t*)value);
        }

#if IL2CPP_ENABLE_INTERLOCKED_64_REQUIRED_ALIGNMENT
        static inline int64_t Increment64(int64_t* value)
        {
            return UnityPalIncrement64(value);
        }

        static inline uint64_t Increment64(uint64_t* value)
        {
            return (uint64_t)Increment64((int64_t*)value);
        }

#endif

        static inline int32_t Decrement(int32_t* value)
        {
            return UnityPalDecrement(value);
        }

        static inline uint32_t Decrement(uint32_t* value)
        {
            return (uint32_t)Decrement((int32_t*)value);
        }

#if IL2CPP_ENABLE_INTERLOCKED_64_REQUIRED_ALIGNMENT
        static inline int64_t Decrement64(int64_t* value)
        {
            return UnityPalDecrement64(value);
        }

        static inline uint64_t Decrement64(uint64_t* value)
        {
            return (uint64_t)Decrement64((int64_t*)value);
        }

#endif

        static inline int32_t CompareExchange(int32_t* dest, int32_t exchange, int32_t comparand)
        {
            return UnityPalCompareExchange(dest, exchange, comparand);
        }

        static inline uint32_t CompareExchange(uint32_t* value, uint32_t newValue, uint32_t oldValue)
        {
            return (uint32_t)CompareExchange((int32_t*)value, newValue, oldValue);
        }

        static inline int64_t CompareExchange64(int64_t* dest, int64_t exchange, int64_t comparand)
        {
            return UnityPalCompareExchange64(dest, exchange, comparand);
        }

        static inline uint64_t CompareExchange64(uint64_t* value, uint64_t newValue, uint64_t oldValue)
        {
            return (uint64_t)CompareExchange64((int64_t*)value, newValue, oldValue);
        }

        static inline bool Exchange(bool* dest, bool exchange)
        {
            return (bool)Baselib_atomic_exchange_8_seq_cst((int8_t*)dest, exchange);
        }

        static inline int32_t Exchange(int32_t* dest, int32_t exchange)
        {
            return UnityPalExchange(dest, exchange);
        }

        static inline uint32_t Exchange(uint32_t* value, uint32_t newValue)
        {
            return (uint32_t)Exchange((int32_t*)value, newValue);
        }

#if IL2CPP_ENABLE_INTERLOCKED_64_REQUIRED_ALIGNMENT
        static inline int64_t Exchange64(int64_t* dest, int64_t exchange)
        {
            return UnityPalExchange64(dest, exchange);
        }

        static inline uint64_t Exchange64(uint64_t* value, uint64_t newValue)
        {
            return (uint64_t)Exchange64((int64_t*)value, newValue);
        }

#endif
    };
}
}
