# include <mdlint.h>
mdl_u8_t ffly_mem_cpy(void *__dst, void *__src, mdl_u32_t __bc) {
	__asm__("mov %0, %%rdi\n\t"
			"mov %1, %%rsi\n\t"
			"xorq %%rbx, %%rbx\n\t"
			"mov %2, %%ebx\n\t"
			"call __ffly_mem_cpy" : : "m"(__dst), "m"(__src), "m"(__bc) : "rdi", "rsi", "rbx");
	return 0;
}
