# include "../ffint.h"
ff_s32_t access(char const *__file, ff_u32_t __mode) {
	ff_s32_t ret;
	__asm__("movq %1, %%rdi\n\t"
			"movl %2, %%esi\n\t" 
			"call __access\n\t"
			"movl %%eax, %0" : "=m"(ret) : "m"(__file), "m"(__mode) : "rdi", "esi", "rax");
	return ret;
}
