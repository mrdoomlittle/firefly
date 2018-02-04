# include <mdlint.h>
# include "../linux/stat.h"
mdl_s32_t ffly_stat(char const *__path, struct __linux_stat *__stat) {
	mdl_s32_t ret;
	__asm__("mov %1, %%rdi\n\t"
			"mov %2, %%rsi\n\t"
			"call _ffly_stat\n\t"
			"mov %%eax, %0" : "=r"(ret) : "r"(__path), "r"(__stat));
	return ret;
}
