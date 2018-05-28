# include "../ffint.h"
ff_s32_t getsockopt(ff_u32_t __fd, ff_u32_t __level, ff_u32_t __name, void *__val, ff_u32_t *__len) {
	ff_s32_t ret;
	__asm__("xorq %%rdi, %%rdi\n\t"
			"movl %1, %%edi\n\t"
			"xorq %%rsi, %%rsi\n\t"
			"movl %2, %%esi\n\t"
			"xorq %%rdx, %%rdx\n\t"
			"movl %3, %%edx\n\t"
			"movq %4, %%r10\n\t"
			"movq %5, %%r8\n\t"
			"call __setsockopt\n\t"
			"movl %%eax, %0" : "=m"(ret) : "m"(__fd), "m"(__level), "m"(__name), "m"(__val), "m"(__len)
				: "rdi", "rsi", "rdx", "r10", "r8", "rax");
	return ret;
}
