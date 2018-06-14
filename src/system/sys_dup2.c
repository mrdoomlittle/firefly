# include "../ffint.h"
ff_s32_t dup2(ff_u32_t __old, ff_u32_t __new) {
	ff_s32_t ret;
	__asm__("xorq %%rdi, %%rdi\n\t"
			"xorq %%rsi, %%rsi\n\t"
			"movl %1, %%edi\n\t"
			"movl %2, %%esi\n\t"
			"call __dup2\n\t"
			"movl %%eax, %0" : "=m"(ret) : "m"(__old), "m"(__new) : "rdi", "rsi", "rax");
	return ret;
}
