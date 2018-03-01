void ffly_str_cpy(char *__dst, char const *__src) {
	__asm__("mov %0, %%rdi\n\t"
			"mov %1, %%rsi\n\t"
			"call __ffly_str_cpy" : : "m"(__dst), "m"(__src));
}
