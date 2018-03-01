# include <mdlint.h>
mdl_i8_t ffly_mem_cmp(void const *__p0, void const *__p1, mdl_u32_t __bc) {
	mdl_i8_t ret;
	__asm__("mov %1, %%rdi\n\t"
			"mov %2, %%rsi\n\t"
			"xorq %%rbx, %%rbx\n\t"
			"mov %3, %%ebx\n\t"
			"call __ffly_mem_cmp\n\t"
			"mov %%al, %0" : "=m"(ret) : "m"(__p0), "m"(__p1), "m"(__bc));
	return ret;
}
