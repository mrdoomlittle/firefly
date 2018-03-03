# include <mdlint.h>
void* ffly_mem_set(void *__dst, mdl_u8_t __val, mdl_u32_t __bc) {
	__asm__("mov %0, %%rdi\n\t"
			"mov %1, %%sil\n\t"
			"xorq %%rbx, %%rbx\n\t"
			"mov %2, %%ebx\n\t"
			"call __ffly_mem_set" : : "m"(__dst), "m"(__val), "m"(__bc) : "rdi", "rsi", "rbx");
	return __dst;
}
