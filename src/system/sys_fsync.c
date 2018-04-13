# include "../ffint.h"
ff_s32_t fsync(ff_s32_t __fd) {
	ff_s32_t ret;
	__asm__("xorq %%rdi, %%rdi\n\t"
			"mov %1, %%edi\n\t"
			"call __fsync\n\t"
			"mov %%eax, %0" : "=m"(ret) : "m"(__fd) : "rdi", "rax");
	return ret;
}
