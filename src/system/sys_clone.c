# include "../ffint.h"
ff_s64_t clone(ff_u64_t __flags, ff_u64_t __newsp, void *__parent, void *__child, ff_u64_t __tls) {
	ff_s64_t ret;
	__asm__("movq %1, %%rdi\n\t"
			"movq %2, %%rsi\n\t"
			"movq %3, %%rdx\n\t"
			"movq %4, %%r10\n\t"
			"movq %5, %%r8\n\t"
			"call __clone\n\t"
			"movq %%rax, %0" : "=m"(ret) : "m"(__flags), "m"(__newsp), "m"(__parent), "m"(__child),
				"m"(__tls) : "rdi", "rsi", "rdx", "r10", "r8", "rax");
	return ret;
}
