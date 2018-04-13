# include "../ffint.h"
ff_u32_t ffly_str_cpy(char *__dst, char const *__src) {
	ff_u32_t l;
	__asm__("mov %1, %%rdi\n\t"
			"mov %2, %%rsi\n\t"
			"call __ffly_str_cpy\n\t"
			"mov %%eax, %0": "=m"(l) : "m"(__dst), "m"(__src) : "rdi", "rsi", "rax");
	return l;
}
