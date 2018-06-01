# include "../ffint.h"
// not tested
ff_s32_t pwrite(ff_u32_t __fd, void *__buf, ff_u64_t __size, ff_u64_t __offset) {
	ff_s32_t ret;
	__asm__("xorq %%rdi, %%rdi\n\t"
			"movl %1, %%edi\n\t"
			"movq %2, %%rsi\n\t"
			"movq %3, %%rdx\n\t"
			"movq %4, %%r10\n\t"
			"call __pwrite\n\t"
			"movl %%eax, %0" : "=m"(ret) : "m"(__fd), "m"(__buf), "m"(__size), "m"(__offset) : "rdi", "rsi", "rdx", "r10", "rax");
	return ret;
}
