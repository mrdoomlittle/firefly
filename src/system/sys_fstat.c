# include "../linux/stat.h"
# include "../linux/types.h"
mdl_s32_t fstat(mdl_s32_t __fd, struct stat *__buf) {
	mdl_s32_t ret;
	__asm__("mov %1, %%rdi\n\t"
			"mov %2, %%rsi\n\t"
			"call _fstat\n\t"
			"mov %%eax, %0" : "=m"(ret) : "m"(__fd), "m"(__buf));
	return ret;
}
