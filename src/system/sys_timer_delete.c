# include "../ffint.h"
# include "../linux/types.h"
ff_s32_t timer_delete(__linux_timer_t __timer) {
	ff_s32_t ret;
	__asm__("xorq %%rdi, %%rdi\n\t"
			"movl %1, %%edi\n\t"
			"call __timer_delete\n\t"
			"movl %%eax, %0" : "=m"(ret) : "m"(__timer) : "rdi", "rax"
	);

	return ret;
}
