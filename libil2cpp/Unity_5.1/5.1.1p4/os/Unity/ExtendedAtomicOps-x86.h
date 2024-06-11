
#if defined (_MSC_VER)
#	include "Windows.h"
#	include <intrin.h>
#endif

#if defined(__SSE2__)
#	include <emmintrin.h>
#endif

static inline atomic_word atomic_load_explicit (const volatile atomic_word* p, int)
{
	// ideally, this should be in assembly to prevent the compiler from optimizing or reordering this
	return *p;
}

static inline void atomic_store_explicit (volatile atomic_word* p, atomic_word v, int)
{
	// ideally, this should be in assembly to prevent the compiler from optimizing or reordering this
	*p = v;
}

static inline void atomic_store_explicit (volatile atomic_word* p, atomic_word val, memory_order_seq_cst_t)
{
#if defined (_MSC_VER)
	_InterlockedExchange ((volatile LONG*) p, (LONG) val);
#else
	// lock prefix is implicit
	__asm__ __volatile__
	(
/*lock*/"xchgl  %1, %0"

		: "+m" (*p), "+r" (val)
		:
		: "memory"
	);
#endif
}

static inline atomic_word atomic_exchange_explicit (volatile atomic_word* p, atomic_word val, int)
{
#if defined (_MSC_VER)
	return (atomic_word) _InterlockedExchange ((volatile LONG*) p, (LONG) val);
#else
	// lock prefix is implicit
	__asm__ __volatile__
	(
/*lock*/"xchgl  %1, %0"

		: "+m" (*p), "+r" (val)
		:
		: "memory"
	);
	return val;
#endif
}

static inline bool atomic_compare_exchange_strong_explicit (volatile atomic_word* p, atomic_word* oldval, atomic_word newval, int, int)
{
#if defined (_MSC_VER)
	atomic_word tmp = (atomic_word) _InterlockedCompareExchange ((volatile LONG*) p, (LONG) newval, (LONG) *oldval);
	return *oldval==tmp ? true : (*oldval=tmp, false);
#else
	char res;
	__asm__ __volatile__
	(
	"lock cmpxchgl %3, %0\n\t"
		 "setz %b1"
	
		: "+m" (*p), "=q" (res), "+a" (*oldval)
		: "r" (newval)
		: "cc", "memory"
	);
	return res != 0;
#endif
}

static inline atomic_word2 atomic_load_explicit (const volatile atomic_word2* p, int)
{
	atomic_word2 r;
#if defined (__SSE2__)
	_mm_store_sd ((double*) &r, _mm_load_sd ((const double*) p));
#else
	// using the FPU is the only way to do a 64 bit atomic load if SSE is not available
	r.d = p->d;
#endif
	return r;
}

static inline void atomic_store_explicit (volatile atomic_word2* p, atomic_word2 v, int)
{
#if defined (__SSE2__)
	_mm_store_sd ((double*) p, _mm_load_sd ((const double*) &v));
#else
	// using the FPU is the only way to do a 64 bit atomic store if SSE is not available
	p->d = v.d;
#endif
}

static inline void atomic_init_safe_explicit (volatile atomic_word2* p, atomic_word v, int)
{
	atomic_word2 w;
	w.lo = v;
	w.hi = 0;
#if defined (__SSE2__)
	_mm_store_sd ((double*) p, _mm_load_sd ((const double*) &w));
#else
	// using the FPU is the only way to do a 64 bit atomic store if SSE is not available
	p->d = w.d;
#endif
}

static inline bool atomic_compare_exchange_strong_explicit (volatile atomic_word2* p, atomic_word2* oldval, atomic_word2 newval, int, int)
{
#if defined (_MSC_VER)
	LONGLONG tmp = _InterlockedCompareExchange64 ((volatile LONGLONG*) p, newval.v, oldval->v);
	return oldval->v==tmp ? true : (oldval->v=tmp, false);
#else
	char res;
	__asm__ __volatile__
	(
"lock cmpxchg8b %0\n\t"
		"setz   %b1\n\t"

		: "+m" (*p), "=q" (res), "+a" (oldval->lo), "+d" (oldval->hi)
		: "b" (newval.lo), "c" (newval.hi)
		: "cc", "memory"
	);
	return res != 0;
#endif
}

static inline bool atomic_compare_exchange_safe_explicit (volatile atomic_word2* p, atomic_word2* oldval, atomic_word newlo, int, int)
{
	atomic_word2 newval;
	newval.lo = newlo;
	newval.hi = oldval->hi + 1;
	return atomic_compare_exchange_strong_explicit (p, oldval, newval, memory_order_seq_cst, memory_order_seq_cst);
}

static inline atomic_word atomic_exchange_safe_explicit (volatile atomic_word2* p, atomic_word v, int)
{
	atomic_word2 oldval = atomic_load_explicit (p, memory_order_relaxed);
	atomic_word2 newval;
	do
	{
		newval.lo = v; newval.hi = oldval.hi + 1;
	}
	while (!atomic_compare_exchange_strong_explicit (p, &oldval, newval, memory_order_seq_cst, memory_order_seq_cst));
	return oldval.lo;
}
