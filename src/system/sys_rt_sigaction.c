# include <mdlint.h>
# include "../linux/signal.h"
mdl_s32_t rt_sigaction(mdl_s32_t __sig, struct sigaction const *__act, struct sigaction *__oact, mdl_u32_t __size) {
	mdl_s32_t ret;
	__asm__("xorq %%rdi, %%rdi\n\t"
			"mov %1, %%edi\n\t"
			"mov %2, %%rsi\n\t"
			"mov %3, %%rdx\n\t"
			"xorq %%r10, %%r10\n\t"
			"mov %4, %%r10d\n\t"
			"call __rt_sigaction\n\t"
			"mov %%eax, %0" : "=m"(ret) : "m"(__sig), "m"(__act), "m"(__oact), "m"(__size) : "rdi", "rsi", "rdx", "r10", "rax");
	return ret;
}
