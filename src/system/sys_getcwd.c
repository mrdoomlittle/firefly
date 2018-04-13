# include "../ffint.h"
ff_s32_t getcwd(char *__buf, ff_u64_t __size) {
	ff_s32_t ret;
	__asm__("mov %1, %%rdi\n\t"
			"mov %2, %%rsi\n\t"
			"call _getcwd\n\t"
			"mov %%eax, %0" : "=m"(ret) : "m"(__buf), "m"(__size) : "rdi", "rsi", "rax");
	return ret;
}
