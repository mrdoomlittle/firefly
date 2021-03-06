# include "../ffint.h"
ff_s32_t socket(ff_u32_t __family, ff_u32_t __type, ff_u32_t __proto) {
	ff_s32_t ret;
	__asm__("xorq %%rdi, %%rdi\n\t"
			"movl %1, %%edi\n\t"
			"xorq %%rsi, %%rsi\n\t"
			"movl %2, %%esi\n\t"
			"xorq %%rdx, %%rdx\n\t"
			"movl %3, %%edx\n\t"
			"call __socket\n\t"
			"movl %%eax, %0" : "=m"(ret) : "m"(__family), "m"(__type), "m"(__proto) : "rdi", "rsi", "rdx", "rax");
	return ret;
}
