# include "../ffint.h"
# include "../linux/stat.h"
# include "../linux/types.h"
ff_s32_t fstat(ff_s32_t __fd, struct stat *__buf) {
	ff_s32_t ret;
	__asm__("xorq %%rdi, %%rdi\n\t"
			"mov %1, %%edi\n\t"
			"mov %2, %%rsi\n\t"
			"call __fstat\n\t"
			"mov %%eax, %0" : "=m"(ret) : "m"(__fd), "m"(__buf) : "rdi", "rsi", "rax");
	return ret;
}
