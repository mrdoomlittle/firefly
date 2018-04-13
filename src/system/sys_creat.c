# include "../ffint.h"
ff_s32_t creat(char const *__path, ff_s32_t __mode) {
	ff_s32_t ret;
	__asm__("mov %1, %%rdi\n\t"
			"mov %2, %%esi\n\t"
			"call __creat\n\t"
			"mov %%eax, %0" : "=m"(ret) : "m"(__path), "m"(__mode) : "rdi", "esi", "rax");
	return ret;
}
