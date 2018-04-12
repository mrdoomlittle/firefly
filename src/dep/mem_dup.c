# include <mdlint.h>
# include "../types/err_t.h"
# include "../system/err.h"
# ifndef __fflib
# include "../memory/mem_alloc.h"
# include <string.h>
# endif
ffly_err_t ffly_mem_dup(void **__p, void const *__src, mdl_u32_t __bc) {
# ifdef __fflib
	__asm__("mov %0, %%rdi\n\t"
			"mov %1, %%rsi\n\t"
			"xorq %%rbx, %%rbx\n\t"
			"mov %2, %%ebx\n\t"
			"call __ffly_mem_dup" : : "m"(__p), "m"(__src), "m"(__bc) : "rdi", "rsi", "rbx");
# else
	memcpy(*__p = __ffly_mem_alloc(__bc), __src, __bc);
# endif
	retok;
}
