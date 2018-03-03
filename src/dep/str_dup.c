char const* ffly_str_dup(char const *__src) {
	char const *ret;
	__asm__("mov %1, %%rdi\n\t"
			"call __ffly_str_dup\n\t"
			"mov %%rax, %0" : "=m"(ret) : "m"(__src) : "rdi", "rax");
	return ret;
}
