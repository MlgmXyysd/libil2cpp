#pragma once

#include "il2cpp-config-platforms.h"
#include <stdint.h>

#if IL2CPP_SUPPORT_THREADS

#include "Baselib.h"
#include "C/Baselib_Atomic_TypeSafe.h"

inline void UnityPalFullMemoryBarrier()
{
    Baselib_atomic_thread_fence_seq_cst();
}

inline int32_t UnityPalAdd(volatile int32_t* location1, int32_t value)
{
    return Baselib_atomic_fetch_add_32_seq_cst(const_cast<int32_t*>(location1), value) + value;
}

#if IL2CPP_ENABLE_INTERLOCKED_64_REQUIRED_ALIGNMENT
inline int64_t UnityPalAdd64(volatile int64_t* location1, int64_t value)
{
    return Baselib_atomic_fetch_add_64_seq_cst(const_cast<int64_t*>(location1), value) + value;
}

#endif

inline int32_t UnityPalIncrement(volatile int32_t* value)
{
    return Baselib_atomic_fetch_add_32_seq_cst(const_cast<int32_t*>(value), 1) + 1;
}

#if IL2CPP_ENABLE_INTERLOCKED_64_REQUIRED_ALIGNMENT
inline int64_t UnityPalIncrement64(volatile int64_t* value)
{
    return Baselib_atomic_fetch_add_64_seq_cst(const_cast<int64_t*>(value), 1) + 1;
}

#endif

inline int32_t UnityPalDecrement(volatile int32_t* value)
{
    return Baselib_atomic_fetch_add_32_seq_cst(const_cast<int32_t*>(value), -1) - 1;
}

#if IL2CPP_ENABLE_INTERLOCKED_64_REQUIRED_ALIGNMENT
inline int64_t UnityPalDecrement64(volatile int64_t* value)
{
    return Baselib_atomic_fetch_add_64_seq_cst(const_cast<int64_t*>(value), -1) - 1;
}

#endif

inline int32_t UnityPalCompareExchange(volatile int32_t* dest, int32_t exchange, int32_t comparand)
{
    Baselib_atomic_compare_exchange_strong_32_seq_cst_seq_cst(const_cast<int32_t*>(dest), &comparand, exchange);
    return comparand;
}

inline int64_t UnityPalCompareExchange64(volatile int64_t* dest, int64_t exchange, int64_t comparand)
{
    Baselib_atomic_compare_exchange_strong_64_seq_cst_seq_cst(const_cast<int64_t*>(dest), &comparand, exchange);
    return comparand;
}

inline void* UnityPalCompareExchangePointer(void* volatile* dest, void* exchange, void* comparand)
{
    Baselib_atomic_compare_exchange_strong_ptr_seq_cst_seq_cst((intptr_t*)const_cast<void**>(dest), (intptr_t*)&comparand, (intptr_t)exchange);
    return comparand;
}

inline int32_t UnityPalExchange(volatile int32_t* dest, int32_t exchange)
{
    return Baselib_atomic_exchange_32_seq_cst(const_cast<int32_t*>(dest), exchange);
}

#if IL2CPP_ENABLE_INTERLOCKED_64_REQUIRED_ALIGNMENT
inline int64_t UnityPalExchange64(volatile int64_t* dest, int64_t exchange)
{
    return Baselib_atomic_exchange_64_seq_cst(const_cast<int64_t*>(dest), exchange);
}

#endif

inline void* UnityPalExchangePointer(void* volatile* dest, void* exchange)
{
    return (void*)Baselib_atomic_exchange_ptr_seq_cst((intptr_t*)const_cast<void**>(dest), (intptr_t)exchange);
}

inline int64_t UnityPalRead64(volatile int64_t* addr)
{
    return Baselib_atomic_fetch_add_64_seq_cst(const_cast<int64_t*>(addr), 0);
}

#else

inline void UnityPalFullMemoryBarrier()
{
}

inline int32_t UnityPalAdd(volatile int32_t* location1, int32_t value)
{
    return *location1 += value;
}

#if IL2CPP_ENABLE_INTERLOCKED_64_REQUIRED_ALIGNMENT
inline int64_t UnityPalAdd64(volatile int64_t* location1, int64_t value)
{
    return *location1 += value;
}

#endif

inline int32_t UnityPalIncrement(volatile int32_t* value)
{
    return ++(*value);
}

#if IL2CPP_ENABLE_INTERLOCKED_64_REQUIRED_ALIGNMENT
inline int64_t UnityPalIncrement64(volatile int64_t* value)
{
    return ++(*value);
}

#endif

inline int32_t UnityPalDecrement(volatile int32_t* value)
{
    return --(*value);
}

#if IL2CPP_ENABLE_INTERLOCKED_64_REQUIRED_ALIGNMENT
inline int64_t UnityPalDecrement64(volatile int64_t* value)
{
    return --(*value);
}

#endif

inline int32_t UnityPalCompareExchange(volatile int32_t* dest, int32_t exchange, int32_t comparand)
{
    int32_t orig = *dest;
    if (*dest == comparand)
        *dest = exchange;

    return orig;
}

inline int64_t UnityPalCompareExchange64(volatile int64_t* dest, int64_t exchange, int64_t comparand)
{
    int64_t orig = *dest;
    if (*dest == comparand)
        *dest = exchange;

    return orig;
}

inline void* UnityPalCompareExchangePointer(void* volatile* dest, void* exchange, void* comparand)
{
    void* orig = *dest;
    if (*dest == comparand)
        *dest = exchange;

    return orig;
}

#if IL2CPP_ENABLE_INTERLOCKED_64_REQUIRED_ALIGNMENT
inline int64_t UnityPalExchange64(volatile int64_t* dest, int64_t exchange)
{
    int64_t orig = *dest;
    *dest = exchange;
    return orig;
}

#endif

inline int32_t UnityPalExchange(volatile int32_t* dest, int32_t exchange)
{
    int32_t orig = *dest;
    *dest = exchange;
    return orig;
}

inline void* UnityPalExchangePointer(void* volatile* dest, void* exchange)
{
    void* orig = *dest;
    *dest = exchange;
    return orig;
}

inline int64_t UnityPalRead64(volatile int64_t* addr)
{
    return *addr;
}

#endif
