# include "../ffint.h"
# ifndef __fflib
# include <string.h>
# endif
void* ffly_mem_set(void *__dst, ff_u8_t __val, ff_u32_t __bc) {
# ifdef __fflib
	__asm__("mov %0, %%rdi\n\t"
			"mov %1, %%sil\n\t"
			"xorq %%rbx, %%rbx\n\t"
			"mov %2, %%ebx\n\t"
			"call __ffly_mem_set" : : "m"(__dst), "m"(__val), "m"(__bc) : "rdi", "rsi", "rbx");
	return __dst;
# else
	return memset(__dst, __val, __bc);
# endif
}
