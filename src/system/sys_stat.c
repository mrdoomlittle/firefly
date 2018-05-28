# include "../ffint.h"
# include "../linux/stat.h"
ff_s32_t stat(char const *__path, struct stat *__buf) {
	ff_s32_t ret;
	__asm__("movq %1, %%rdi\n\t"
			"movq %2, %%rsi\n\t"
			"call __stat\n\t"
			"movl %%eax, %0" : "=m"(ret) : "m"(__path), "m"(__buf) : "rdi", "rsi", "rax");
	return ret;
}
