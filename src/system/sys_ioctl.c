# include "../ffint.h"
ff_s32_t ioctl(ff_u32_t __fd, ff_u64_t __cmd, ff_u64_t __arg) {
	ff_s32_t ret;
	__asm__("xorq %%rdi, %%rdi\n\t"
			"movl %1, %%edi\n\t"
			"movq %2, %%rsi\n\t"
			"movq %3, %%rdx\n\t"
			"call __ioctl\n"
			"movl %%eax, %0" : "=m"(ret) :  "m"(__fd), "m"(__cmd), "m"(__arg) : "rdi", "rsi", "rdx", "rax");
	return ret;
}
