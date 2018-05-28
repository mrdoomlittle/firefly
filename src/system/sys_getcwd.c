# include "../ffint.h"
ff_s32_t getcwd(char *__buf, ff_u64_t __size) {
	ff_s32_t ret;
	__asm__("movq %1, %%rdi\n\t"
			"movq %2, %%rsi\n\t"
			"call _getcwd\n\t"
			"movl %%eax, %0" : "=m"(ret) : "m"(__buf), "m"(__size) : "rdi", "rsi", "rax");
	return ret;
}
