# include "../ffint.h"
ff_s32_t lseek(ff_u32_t __fd, ff_u64_t __off, ff_u32_t __whence) {
	ff_s32_t ret;
	__asm__("xorq %%rdi, %%rdi\n\t"
			"movl %1, %%edi\n\t"
			"movq %2, %%rsi\n\t"
			"movl %3, %%edx\n\t"
			"call __lseek\n\t"
			"movl %%eax, %0" : "=m"(ret) : "m"(__fd), "m"(__off), "m"(__whence) : "rdi", "rsi", "edx", "rax");
	return ret;
}
