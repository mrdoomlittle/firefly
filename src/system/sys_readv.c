# include "../ffint.h"
# include "../linux/uio.h"
ff_s32_t readv(ff_u32_t __fd, struct iovec *__vec, ff_u32_t __vlen) {
	ff_u32_t ret;
	__asm__("xorq %%rdi, %%rdi\n\t"
		"movl %1, %%edi\n\t"
		"movq %2, %%rsi\n\t"
		"xorq %%rdx, %%rdx\n\t"
		"movl %3, %%rdx\n\t"
		"call __readv\n\t"
		"movl %%eax, %0": "=m"(ret) : "m"(__fd), "m"(__vec), "m"(__vlen) : "rdi", "rsi", "rdx", "rax");
	return ret;
}
