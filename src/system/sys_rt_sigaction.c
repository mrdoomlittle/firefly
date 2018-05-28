# include "../ffint.h"
# include "../linux/signal.h"
ff_s32_t rt_sigaction(ff_s32_t __sig, struct sigaction const *__act, struct sigaction *__oact, ff_u32_t __size) {
	ff_s32_t ret;
	__asm__("xorq %%rdi, %%rdi\n\t"
			"movl %1, %%edi\n\t"
			"movq %2, %%rsi\n\t"
			"movq %3, %%rdx\n\t"
			"xorq %%r10, %%r10\n\t"
			"movl %4, %%r10d\n\t"
			"call __rt_sigaction\n\t"
			"movl %%eax, %0" : "=m"(ret) : "m"(__sig), "m"(__act), "m"(__oact), "m"(__size) : "rdi", "rsi", "rdx", "r10", "rax");
	return ret;
}
