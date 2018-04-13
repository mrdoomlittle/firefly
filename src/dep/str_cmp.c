# include "../ffint.h"
ff_i8_t ffly_str_cmp(char const *__s0, char const *__s1) {
	ff_i8_t ret;
	__asm__("mov %1, %%rdi\n\t"
			"mov %2, %%rsi\n\t"
			"call __ffly_str_cmp\n\t"
			"movb %%al, %0" : "=m"(ret) : "m"(__s0), "m"(__s1) : "rdi", "rsi", "rax");
	return ret;
}
