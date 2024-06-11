
#if defined (_MSC_VER)
#	include "Windows.h"
#	include <intrin.h>
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
	_InterlockedExchange64 ((volatile LONGLONG*) p, (LONGLONG) val);
#else
	// lock prefix is implicit
	__asm__ __volatile__
	(
/*lock*/"xchgq  %1, %0"

		: "+m" (*p), "+r" (val)
		:
		: "memory"
	);
#endif
}

static inline atomic_word atomic_exchange_explicit (volatile atomic_word* p, atomic_word val, int)
{
#if defined (_MSC_VER)
	return (atomic_word) _InterlockedExchange64 ((volatile LONGLONG*) p, (LONGLONG) val);
#else
	// lock prefix is implicit
	__asm__ __volatile__
	(
/*lock*/"xchgq  %1, %0"

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
	atomic_word tmp = (atomic_word) _InterlockedCompareExchange64 ((volatile LONGLONG*) p, (LONGLONG) newval, (LONGLONG) *oldval);
	return *oldval==tmp ? true : (*oldval=tmp, false);
#else
	char res;
	__asm__ __volatile__
	(
	"lock cmpxchgq %3, %0\n\t"
		 "setz %b1"
	
		: "+m" (*p), "=q" (res), "+a" (*oldval)
		: "r" (newval)
		: "cc", "memory"
	);
	return res != 0;
#endif
}

static inline void atomic_init_safe_explicit (volatile atomic_word2* p, atomic_word v, int)
{
	atomic_word2 w;
	w.lo = v;
	w.hi = 0;
	_mm_store_si128 ((__m128i*) &p->v, w.v);
}

static inline bool atomic_compare_exchange_strong_explicit (volatile atomic_word2* p, atomic_word2* oldval, atomic_word2 newval, int, int)
{
#if defined (_MSC_VER)
	return _InterlockedCompareExchange128 ((volatile LONGLONG*) p, (LONGLONG) newval.hi, (LONGLONG) newval.lo, (LONGLONG*) oldval) != 0;
#else
	char res;
	__asm__ __volatile__
	(
"lock cmpxchg16b %0\n\t"
		"setz   %b1\n\t"

		: "+m" (*p), "=q" (res), "+a" (oldval->lo), "+d" (oldval->hi)
		: "b" (newval.lo), "c" (newval.hi)
		: "cc", "memory"
	);
	return res != 0;
#endif
}

static inline atomic_word2 atomic_load_explicit (const volatile atomic_word2* p, int o)
{
/*
	atomic_word2 r = { 0, 0 };
    atomic_word2 c = { 0, 0 };
    atomic_compare_exchange_strong_explicit((volatile atomic_word2*) p, &r, c, o, o);
    return r;
*/
	atomic_word2 r;
	r.v = _mm_load_si128 ((const __m128i*) p);
	return r;
}

static inline void atomic_store_explicit (volatile atomic_word2* p, atomic_word2 v, int o)
{
/*
    atomic_word2 c = v;
    while(!atomic_compare_exchange_strong_explicit(p, &c, v, o, o)) {};
*/
	_mm_store_si128 ((__m128i*) &p->v, v.v);
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
		newval.lo = v;
		newval.hi = oldval.hi + 1;
	}
	while (!atomic_compare_exchange_strong_explicit (p, &oldval, newval, memory_order_seq_cst, memory_order_seq_cst));
	return oldval.lo;
}
