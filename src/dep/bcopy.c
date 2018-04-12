# include <mdlint.h>
# ifndef __fflib
# include <string.h>
# endif
void ffly_bcopy(void *__dst, void const *__src, mdl_u32_t __bc) {
# ifdef __fflib
	__asm__("mov %0, %%rdi\n\t"
			"mov %1, %%rsi\n\t"
			"xorq %%rbx, %%rbx\n\t"
			"mov %2, %%ebx\n\t"
			"call __ffly_bcopy" : : "m"(__dst), "m"(__src), "m"(__bc) : "rdi", "rsi", "rbx");
# else
	bcopy(__src, __dst, __bc);
# endif
}
