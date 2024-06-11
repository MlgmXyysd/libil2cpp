
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
		"ldr	%0, %1\n\t"
		"dmb	ish\n\t"

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
		"dmb	ishst\n\t"
		"str	%1, %0\n\t"

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
		"dmb	ishst\n\t"
	"0:\n\t"
		"ldrex	%2, [%4]\n\t"
		"strex	%0, %3, [%4]\n\t"
		"teq	%0, #0\n\t"
		"bne	0b\n\t"
	
		: "=&r" (success), "+m" (*p), "=&r" (res)
		: "r" (v), "r" (p)
		: "cc", "memory"
	);
	return res;
}

/*
 *	double word
 */

// Note: the only way to get atomic 64-bit memory accesses on ARM is to use ldrexd/strexd with a loop
// (ldrd and strd instructions are not guaranteed to appear atomic)

static inline atomic_word2 atomic_load_explicit (const volatile atomic_word2* p, memory_order_relaxed_t)
{
	register atomic_word lo __asm__ ("r2");
	register atomic_word hi __asm__ ("r3");
	atomic_word success;
	
	__asm__ __volatile__
	(
	"0:\n\t"
		"ldrexd	%1, %2, [%3]\n\t"
		"strexd	%0, %1, %2, [%3]\n\t"
		"teq	%0, #0\n\t"
		"bne	0b\n\t"
	
		: "=&r" (success), "=&r" (lo), "=&r" (hi)
		: "r" (p)
		: "cc", "r2", "r3"
	);
	atomic_word2 w;
	w.lo = lo;
	w.hi = hi;
	return w;
}

static inline atomic_word2 atomic_load_explicit (const volatile atomic_word2* p, memory_order_acquire_t)
{
	register atomic_word lo __asm__ ("r2");
	register atomic_word hi __asm__ ("r3");
	atomic_word success;
	
	__asm__ __volatile__
	(
	"0:\n\t"
		"ldrexd	%1, %2, [%3]\n\t"
		"strexd	%0, %1, %2, [%3]\n\t"
		"teq	%0, #0\n\t"
		"bne	0b\n\t"
		"isb\n\t"
	
		: "=&r" (success), "=&r" (lo), "=&r" (hi)
		: "r" (p)
		: "cc", "r2", "r3"
	);
	atomic_word2 w;
	w.lo = lo;
	w.hi = hi;
	return w;
}

static inline void atomic_init_safe_explicit (volatile atomic_word2* p, atomic_word v, memory_order_relaxed_t)
{
	register atomic_word l __asm__ ("r2");
	register atomic_word h __asm__ ("r3");
	register atomic_word lo __asm__ ("r0") = v;
	register atomic_word hi __asm__ ("r1") = 0;
	atomic_word success;
	
	__asm__ __volatile__
	(
	"0:\n\t"
		"ldrexd	%2, %3, [%6]\n\t"
		"strexd	%0, %4, %5, [%6]\n\t"
		"teq	%0, #0\n\t"
		"bne	0b\n\t"
	
		: "=&r" (success), "=m" (*p), "=&r" (l), "=&r" (h)
		: "r" (lo), "r" (hi), "r" (p)
		: "cc", "memory", "r2", "r3"
	);
}

static inline void atomic_store_explicit (volatile atomic_word2* p, atomic_word2 v, memory_order_relaxed_t)
{
	register atomic_word l __asm__ ("r2");
	register atomic_word h __asm__ ("r3");
	register atomic_word lo __asm__ ("r0") = v.lo;
	register atomic_word hi __asm__ ("r1") = v.hi;
	atomic_word success;
	
	__asm__ __volatile__
	(
	"0:\n\t"
		"ldrexd	%2, %3, [%6]\n\t"
		"strexd	%0, %4, %5, [%6]\n\t"
		"teq	%0, #0\n\t"
		"bne	0b\n\t"
	
		: "=&r" (success), "=m" (*p), "=&r" (l), "=&r" (h)
		: "r" (lo), "r" (hi), "r" (p)
		: "cc", "memory", "r2", "r3"
	);
}

static inline void atomic_store_explicit (volatile atomic_word2* p, atomic_word2 v, memory_order_release_t)
{
	register atomic_word l __asm__ ("r2");
	register atomic_word h __asm__ ("r3");
	register atomic_word lo __asm__ ("r0") = v.lo;
	register atomic_word hi __asm__ ("r1") = v.hi;
	atomic_word success;
	
	__asm__ __volatile__
	(
		"dmb	ishst\n\t"
	"0:\n\t"
		"ldrexd	%2, %3, [%6]\n\t"
		"strexd	%0, %4, %5, [%6]\n\t"
		"teq	%0, #0\n\t"
		"bne	0b\n\t"
	
		: "=&r" (success), "=m" (*p), "=&r" (l), "=&r" (h)
		: "r" (lo), "r" (hi), "r" (p)
		: "cc", "memory", "r2", "r3"
	);
}

static inline atomic_word atomic_exchange_safe_explicit (volatile atomic_word2* p, atomic_word newval, memory_order_acquire_t)
{
	register atomic_word l __asm__ ("r0");
	register atomic_word h __asm__ ("r1");
	register atomic_word lo __asm__ ("r2") = newval;
	register atomic_word hi __asm__ ("r3");
	atomic_word success;
	
	__asm__ __volatile__
	(
	"0:\n\t"
		"ldrexd	%2, %3, [%6]\n\t"
		"add	%4, %3, 1\n\t"
		"strexd	%0, %5, %4, [%6]\n\t"
		"teq	%0, #0\n\t"
		"bne	0b\n\t"
		"isb\n\t"
	
		: "=&r" (success), "=m" (*p), "=&r" (l), "=&r" (h), "=&r" (hi)
		: "r" (lo), "r" (p)
		: "cc", "memory", "r0", "r1", "r3"
	);
	return l;
}

static inline bool atomic_compare_exchange_safe_explicit (volatile atomic_word2* p, atomic_word2* oldval, atomic_word newval, memory_order_acquire_t, memory_order_relaxed_t)
{
	register atomic_word l __asm__ ("r2");
	register atomic_word h __asm__ ("r3");
	register atomic_word lo __asm__ ("r0") = newval;
	register atomic_word hi __asm__ ("r1") = oldval->hi + 1;
	atomic_word success;
	
	__asm__ __volatile__
	(
	"0:\n\t"
		"ldrexd	%2, %3, [%8]\n\t"
		"teq	%3, %5\n\t"
		"it		eq\n\t"
		"teqeq	%2, %4\n\t"
		"bne	1f\n\t"
		"strexd	%0, %6, %7, [%8]\n\t"
		"teq	%0, #0\n\t"
		"bne	0b\n\t"
		"isb\n\t"
	"1:\n\t"
	
		: "=&r" (success), "+m" (*p), "=&r" (l), "=&r" (h)
		: "r" (oldval->lo), "r" (oldval->hi), "r" (lo), "r" (hi), "r" (p), "0" (1)
		: "cc", "memory", "r2", "r3"
	);
	if(success != 0)
	{
		oldval->lo = l;
		oldval->hi = h;
	}
	return success == 0;
}

static inline bool atomic_compare_exchange_safe_explicit (volatile atomic_word2* p, atomic_word2* oldval, atomic_word newval, memory_order_release_t, memory_order_relaxed_t)
{
	register atomic_word l __asm__ ("r2");
	register atomic_word h __asm__ ("r3");
	register atomic_word lo __asm__ ("r0") = newval;
	register atomic_word hi __asm__ ("r1") = oldval->hi + 1;
	atomic_word success;
	
	__asm__ __volatile__
	(
		"dmb	ishst\n\t"
	"0:\n\t"
		"ldrexd	%2, %3, [%8]\n\t"
		"teq	%3, %5\n\t"
		"it		eq\n\t"
		"teqeq	%2, %4\n\t"
		"bne	1f\n\t"
		"strexd	%0, %6, %7, [%8]\n\t"
		"teq	%0, #0\n\t"
		"bne	0b\n\t"
	"1:\n\t"
	
		: "=&r" (success), "+m" (*p), "=&r" (l), "=&r" (h)
		: "r" (oldval->lo), "r" (oldval->hi), "r" (lo), "r" (hi), "r" (p), "0" (1)
		: "cc", "memory", "r2", "r3"
	);
	if(success != 0)
	{
		oldval->lo = l;
		oldval->hi = h;
	}
	return success == 0;
}

static inline bool atomic_compare_exchange_safe_explicit (volatile atomic_word2* p, atomic_word2* oldval, atomic_word newval, memory_order_seq_cst_t, memory_order_relaxed_t)
{
	register atomic_word l __asm__ ("r2");
	register atomic_word h __asm__ ("r3");
	register atomic_word lo __asm__ ("r0") = newval;
	register atomic_word hi __asm__ ("r1") = oldval->hi + 1;
	atomic_word success;
	
	__asm__ __volatile__
	(
		"dmb	ishst\n\t"
	"0:\n\t"
		"ldrexd	%2, %3, [%8]\n\t"
		"teq	%3, %5\n\t"
		"it		eq\n\t"
		"teqeq	%2, %4\n\t"
		"bne	1f\n\t"
		"strexd	%0, %6, %7, [%8]\n\t"
		"teq	%0, #0\n\t"
		"bne	0b\n\t"
		"isb\n\t"
	"1:\n\t"
	
		: "=&r" (success), "+m" (*p), "=&r" (l), "=&r" (h)
		: "r" (oldval->lo), "r" (oldval->hi), "r" (lo), "r" (hi), "r" (p), "0" (1)
		: "cc", "memory", "r2", "r3"
	);
	if(success != 0)
	{
		oldval->lo = l;
		oldval->hi = h;
	}
	return success == 0;
}
