# include "../ffint.h"
# include "../linux/time.h"
# include "../linux/types.h"
ff_s32_t timer_settime(__linux_timer_t __timer, ff_u32_t __flags, struct itimerspec *__new, struct itimerspec *__old) {
	ff_s32_t ret;
	__asm__("xorq %%rdi, %%rdi\n\t"
			"movl %1, %%edi\n\t"
			"xorq %%rsi, %%rsi\n\t"
			"movl %2, %%esi\n\t"
			"movq %3, %%rdx\n\t"
			"movq %4, %%r10\n\t"
			"call __timer_settime\n\t"
			"movl %%eax, %0" : "=m"(ret) : "m"(__timer), "m"(__flags), "m"(__new), "m"(__old) : "rdi", "rsi", "rdx", "r10", "rax"
	);

	return ret;
}
