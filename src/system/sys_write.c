# include "../ffint.h"
ff_s32_t write(ff_u32_t __fd, void *__buf, ff_u64_t __size) {
	ff_s32_t ret;
	__asm__("xorq %%rax, %%rax\n\t"
		"movl %1, %%edi\n\t"
        "movq %2, %%rsi\n\t"
        "movq %3, %%rdx\n\t"
        "call __write\n\t"
		"movl %%eax, %0" : "=m"(ret) : "m"(__fd), "m"(__buf), "m"(__size) : "rdi", "rsi", "rdx", "rax");
	return ret;
}
