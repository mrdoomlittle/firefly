# include <mdlint.h>
# include "../linux/stat.h"
mdl_s32_t stat(char const *__path, struct stat *__buf) {
	mdl_s32_t ret;
	__asm__("mov %1, %%rdi\n\t"
			"mov %2, %%rsi\n\t"
			"call _stat\n\t"
			"mov %%eax, %0" : "=m"(ret) : "m"(__path), "m"(__buf) : "rdi", "rsi", "rax");
	return ret;
}
