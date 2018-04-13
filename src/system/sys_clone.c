# include "../ffint.h"
ff_s64_t clone(ff_u64_t __flags, ff_u64_t __newsp, void *__parent, void *__child, ff_u64_t __tls) {
	ff_s64_t ret;
	__asm__("mov %1, %%rdi\n\t"
			"mov %2, %%rsi\n\t"
			"mov %3, %%rdx\n\t"
			"mov %4, %%r10\n\t"
			"mov %5, %%r8\n\t"
			"call __clone\n\t"
			"mov %%rax, %0" : "=m"(ret) : "m"(__flags), "m"(__newsp), "m"(__parent), "m"(__child),
				"m"(__tls) : "rdi", "rsi", "rdx", "r10", "r8", "rax");
	return ret;
}
