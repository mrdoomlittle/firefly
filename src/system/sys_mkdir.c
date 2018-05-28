# include "../ffint.h"
# include "../linux/types.h"
ff_s32_t mkdir(char const *__dir, __linux_mode_t __mode) {
	ff_s32_t ret;
	__asm__("movq %1, %%rdi\n\t"
			"xorq %%rsi, %%rsi\n\t"
			"movl %2, %%esi\n\t"
			"call __mkdir\n\t"
			"movl %%eax, %0" : "=m"(ret) : "m"(__dir), "m"(__mode));
	return ret;
}
