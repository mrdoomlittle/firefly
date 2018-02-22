# include <mdlint.h>
mdl_uint_t ffly_str_len(char const *__s) {
	mdl_u32_t l;
	__asm__("mov %1, %%rdi\n\t"
			"call __ffly_str_len\n\t"
			"mov %%eax, %0" : "=m"(l) : "m"(__s));
	return l;
}
