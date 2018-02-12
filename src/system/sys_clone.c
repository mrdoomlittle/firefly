# include <mdlint.h>
mdl_s64_t clone(mdl_u64_t __flags, mdl_u64_t __newsp, void *__parent, void *__child, mdl_u64_t __tls) {
	mdl_s64_t ret;
	__asm__("mov %1, %%rdi\n\t"
			"mov %2, %%rsi\n\t"
			"mov %3, %%rdx\n\t"
			"mov %4, %%r10\n\t"
			"mov %5, %%r8\n\t"
			"call _clone\n\t"
			"mov %%rax, %0" : "=r"(ret) : "r"(__flags), "r"(__newsp), "r"(__parent), "r"(__child), "r"(__tls));
	return ret;
}
