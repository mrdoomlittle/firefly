# include "../ffint.h"
ff_s32_t access(char const *__file, ff_u32_t __mode) {
	ff_s32_t ret;
	__asm__("mov %1, %%rdi\n\t"
			"mov %2, %%esi\n\t" 
			"call __access\n\t"
			"mov %%eax, %0" : "=m"(ret) : "m"(__file), "m"(__mode) : "rdi", "esi", "rax");
	return ret;
}
