#pragma once

#include "ExtendedAtomicTypes.h"

UNITY_PLATFORM_BEGIN_NAMESPACE;

enum memory_order_relaxed_t { memory_order_relaxed = 0 };
enum memory_order_consume_t { memory_order_consume = 1 };
enum memory_order_acquire_t { memory_order_acquire = 2 };
enum memory_order_release_t	{ memory_order_release = 3 };
enum memory_order_acq_rel_t	{ memory_order_acq_rel = 4 };
enum memory_order_seq_cst_t	{ memory_order_seq_cst = 5 };

#if IL2CPP_TARGET_HAS_EXTENDED_ATOMICS

#	include "os/ExtendedAtomicOps.h"

#elif defined (__x86_64__) || defined (_M_X64)

#	include "ExtendedAtomicOps-x86-64.h"

#elif defined (__x86__) || defined (__i386__) || defined (_M_IX86)

#	include "ExtendedAtomicOps-x86.h"

#elif defined (__arm64__) && (defined(__clang__) || defined(__GNUC__))

#	include "ExtendedAtomicOps-arm64.h"

#elif defined (__arm__) && (defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)) && (!UNITY_BLACKBERRY) && (!UNITY_STV_API) && (!UNITY_TIZEN) && (defined (__clang__) || defined (__GNUC__))

#	include "ExtendedAtomicOps-arm.h"

#elif UNITY_PS3

#	include "ExtendedAtomicOps-ppc.h"

#elif UNITY_XENON

#	include "PlatformDependent/Xbox360/Source/Threads/ExtendedAtomicOps-xenon.h"

#elif UNITY_PSP2

#	include "PlatformDependent/PSP2Player/Source/Threads/ExtendedAtomicOps-psp2.h"

#elif (defined (__ppc64__) || defined (_ARCH_PPC64)) && (defined (__clang__) || defined (__GNUC__))

#	include "ExtendedAtomicOps-ppc64.h"

//#elif defined (__ppc__) && (defined (__clang__) || defined (__GNUC__))

//#	include "Runtime/Threads/ExtendedAtomicOps-ppc.h"

#else

	static inline atomic_word atomic_load_explicit (const volatile atomic_word* p, memory_order_relaxed_t)
	{
		return *p;
	}

	static inline void atomic_store_explicit (volatile atomic_word* p, atomic_word v, memory_order_relaxed_t)
	{
		*p = v;
	}

#endif

UNITY_PLATFORM_END_NAMESPACE;
