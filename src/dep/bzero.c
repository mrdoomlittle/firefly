# include "../ffint.h"
void ffly_bzero(void *__p, ff_u32_t __bc) {
	__asm__("movq %0, %%rdi\n\t"
			"xorq %%rbx, %%rbx\n\t"
			"movl %1, %%ebx\n\t"
			"call __ffly_bzero" : : "m"(__p), "m"(__bc) : "rdi", "rbx");
}
