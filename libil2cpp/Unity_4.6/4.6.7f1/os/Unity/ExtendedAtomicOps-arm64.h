
static inline atomic_word atomic_load_explicit (const volatile atomic_word* p, memory_order_relaxed_t)
{
	// ideally, this should be in assembly to prevent the compiler from optimizing or reordering this
	return *p;
}

static inline atomic_word atomic_load_explicit (const volatile atomic_word* p, memory_order_acquire_t)
{
	atomic_word res;
	__asm__ __volatile__
	(
		"ldar	%0, %1\n\t"

		: "=r" (res)
		: "m" (*p)
	);
	return res;
}

static inline void atomic_store_explicit (volatile atomic_word* p, atomic_word v, memory_order_relaxed_t)
{
	// ideally, this should be in assembly to prevent the compiler from optimizing or reordering this
	*p = v;
}

static inline void atomic_store_explicit (volatile atomic_word* p, atomic_word v, memory_order_release_t)
{
	__asm__ __volatile__
	(
		"stlr	%1, %0\n\t"

		: "=m" (*p)
		: "r" (v)
		: "memory"
	);
}

static inline atomic_word atomic_exchange_explicit (volatile atomic_word* p, atomic_word v, memory_order_release_t)
{
	atomic_word res;
	atomic_word success;
	__asm__ __volatile__
	(
	"0:\n\t"
		"ldxr	%2, [%4]\n\t"
		"stlxr	%w0, %3, [%4]\n\t"
		"cbnz	%w0, 0b\n\t"
	
		: "=&r" (success), "+m" (*p), "=&r" (res)
		: "r" (v), "r" (p)
		: "memory"
	);
	
	return res;
}

/*
 *	double word
 */

// Note: the only way to get atomic 128-bit memory accesses on ARM64 is to use ldxp/stxp with a loop
// (ldxp and stxp instructions are not guaranteed to appear atomic)

static inline atomic_word2 atomic_load_explicit (const volatile atomic_word2* p, memory_order_relaxed_t)
{
	atomic_word2 v;
	atomic_word success;
	__asm__ __volatile__
	(
	"0:\n\t"
		"ldxp	%1, %2, [%3]\n\t"
		"stxp	%w0, %1, %2, [%3]\n\t"
		"cbnz	%w0, 0b\n\t"
	
		: "=&r" (success), "=&r" (v.lo), "=&r" (v.hi)
		: "r" (p)
	);
	return v;
}

static inline atomic_word2 atomic_load_explicit (const volatile atomic_word2* p, memory_order_acquire_t)
{
	atomic_word2 v;
	atomic_word success;
	__asm__ __volatile__
	(
	"0:\n\t"
		"ldaxp	%1, %2, [%3]\n\t"
		"stxp	%w0, %1, %2, [%3]\n\t"
		"cbnz	%w0, 0b\n\t"
	
		: "=&r" (success), "=&r" (v.lo), "=&r" (v.hi)
		: "r" (p)
	);
	return v;
}

static inline void atomic_init_safe_explicit (volatile atomic_word2* p, atomic_word v, memory_order_relaxed_t)
{
	atomic_word lo;
	atomic_word hi;
	atomic_word success;
	__asm__ __volatile__
	(
	"0:\n\t"
		"ldxp	%2, %3, [%6]\n\t"
		"stxp	%w0, %4, %5, [%6]\n\t"
		"cbnz	%w0, 0b\n\t"
	
		: "=&r" (success), "=m" (*p), "=&r" (lo), "=&r" (hi)
		: "r" (v), "r" ((atomic_word) 0), "r" (p)
		: "memory"
	);
}

static inline void atomic_store_explicit (volatile atomic_word2* p, atomic_word2 v, memory_order_relaxed_t)
{
	atomic_word lo;
	atomic_word hi;
	atomic_word success;
	__asm__ __volatile__
	(
	"0:\n\t"
		"ldxp	%2, %3, [%6]\n\t"
		"stxp	%w0, %4, %5, [%6]\n\t"
		"cbnz	%w0, 0b\n\t"
	
		: "=&r" (success), "=m" (*p), "=&r" (lo), "=&r" (hi)
		: "r" (v.lo), "r" (v.hi), "r" (p)
		: "memory"
	);
}

static inline void atomic_store_explicit (volatile atomic_word2* p, atomic_word2 v, memory_order_release_t)
{
	atomic_word lo;
	atomic_word hi;
	atomic_word success;
	__asm__ __volatile__
	(
	"0:\n\t"
		"ldxp	%2, %3, [%6]\n\t"
		"stlxp	%w0, %4, %5, [%6]\n\t"
		"cbnz	%w0, 0b\n\t"
	
		: "=&r" (success), "=m" (*p), "=&r" (lo), "=&r" (hi)
		: "r" (v.lo), "r" (v.hi), "r" (p)
		: "memory"
	);
}

static inline atomic_word atomic_exchange_safe_explicit (volatile atomic_word2* p, atomic_word newval, memory_order_acquire_t)
{
	atomic_word2 oldval;
	atomic_word hi;
	atomic_word success;
	__asm__ __volatile__
	(
	"0:\n\t"
		"ldaxp	%2, %3, [%6]\n\t"
		"add	%4, %3, 1\n\t"
		"stxp	%w0, %5, %4, [%6]\n\t"
		"cbnz	%w0, 0b\n\t"
	
		: "=&r" (success), "+m" (*p), "=&r" (oldval.lo), "=&r" (oldval.hi), "=&r" (hi)
		: "r" (newval), "r" (p)
		: "memory"
	);

	return oldval.lo;
}

static inline bool atomic_compare_exchange_safe_explicit (volatile atomic_word2* p, atomic_word2* oldval, atomic_word newval, memory_order_acquire_t, memory_order_relaxed_t)
{
	atomic_word lo = oldval->lo;
	atomic_word hi = oldval->hi;
	atomic_word success;
	__asm__ __volatile__
	(
	"0:\n\t"
		"ldaxp	%2, %3, [%8]\n\t"
		"cmp	%3, %5\n\t"
		"b.ne	1f\n\t"
		"cmp	%2, %4\n\t"
		"b.ne	1f\n\t"
		"stxp	%w0, %6, %7, [%8]\n\t"
		"cbnz	%w0, 0b\n\t"
	"1:\n\t"
	
		: "=&r" (success), "+m" (*p), "=&r" (oldval->lo), "=&r" (oldval->hi)
		: "r" (lo), "r" (hi), "r" (newval), "r" (hi + 1), "r" (p), "0" (1)
		: "cc", "memory"
	);

	return success == 0;
}

static inline bool atomic_compare_exchange_safe_explicit (volatile atomic_word2* p, atomic_word2* oldval, atomic_word newval, memory_order_release_t, memory_order_relaxed_t)
{
	atomic_word lo = oldval->lo;
	atomic_word hi = oldval->hi;
	atomic_word success;
	__asm__ __volatile__
	(
	"0:\n\t"
		"ldxp	%2, %3, [%8]\n\t"
		"cmp	%3, %5\n\t"
		"b.ne	1f\n\t"
		"cmp	%2, %4\n\t"
		"b.ne	1f\n\t"
		"stlxp	%w0, %6, %7, [%8]\n\t"
		"cbnz	%w0, 0b\n\t"
	"1:\n\t"
	
		: "=&r" (success), "+m" (*p), "=&r" (oldval->lo), "=&r" (oldval->hi)
		: "r" (lo), "r" (hi), "r" (newval), "r" (hi + 1), "r" (p), "0" (1)
		: "cc", "memory"
	);

	return success == 0;
}

static inline bool atomic_compare_exchange_safe_explicit (volatile atomic_word2* p, atomic_word2* oldval, atomic_word newval, memory_order_seq_cst_t, memory_order_relaxed_t)
{
	atomic_word lo = oldval->lo;
	atomic_word hi = oldval->hi;
	atomic_word success;
	__asm__ __volatile__
	(
	"0:\n\t"
		"ldaxp	%2, %3, [%8]\n\t"
		"cmp	%3, %5\n\t"
		"b.ne	1f\n\t"
		"cmp	%2, %4\n\t"
		"b.ne	1f\n\t"
		"stlxp	%w0, %6, %7, [%8]\n\t"
		"cbnz	%w0, 0b\n\t"
	"1:\n\t"
	
		: "=&r" (success), "+m" (*p), "=&r" (oldval->lo), "=&r" (oldval->hi)
		: "r" (lo), "r" (hi), "r" (newval), "r" (hi + 1), "r" (p), "0" (1)
		: "cc", "memory"
	);

	return success == 0;
}
