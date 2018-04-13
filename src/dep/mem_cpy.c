# include "../ffint.h"
# ifndef __fflib
# include <string.h>
# endif
ff_u8_t ffly_mem_cpy(void *__dst, void const *__src, ff_u32_t __bc) {
# ifdef __fflib
	__asm__("mov %0, %%rdi\n\t"
			"mov %1, %%rsi\n\t"
			"xorq %%rbx, %%rbx\n\t"
			"mov %2, %%ebx\n\t"
			"call __ffly_mem_cpy" : : "m"(__dst), "m"(__src), "m"(__bc) : "rdi", "rsi", "rbx");
# else
	memcpy(__dst, __src, __bc);
# endif
	return 0;
}
