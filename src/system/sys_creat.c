# include "../ffint.h"
ff_s32_t creat(char const *__path, ff_u32_t __mode) {
	ff_s32_t ret;
	__asm__("movq %1, %%rdi\n\t"
			"movl %2, %%esi\n\t"
			"call __creat\n\t"
			"movl %%eax, %0" : "=m"(ret) : "m"(__path), "m"(__mode) : "rdi", "esi", "rax");
	return ret;
}
