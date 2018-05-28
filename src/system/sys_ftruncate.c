# include "../ffint.h"
ff_s32_t ftruncate(ff_u32_t __fd, ff_u32_t __len) {
	ff_s32_t ret;
	__asm__("xorq %%rdi, %%rdi\n\t"
			"movl %1, %%edi\n\t"
			"xorq %%rsi, %%rsi\n\t"
			"movl %2, %%esi\n\t"
			"call __ftruncate\n\t"
			"movl %%eax, %0" : "=m"(ret) : "m"(__fd), "m"(__len) : "rdi", "rsi", "rax");
	return ret;
}
