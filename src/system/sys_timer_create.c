# include "../ffint.h"
# include "../linux/types.h"
# include "../linux/signal.h"
# include "../linux/time.h"
ff_s32_t timer_create(__linux_clockid_t __clock, struct sigevent *__event, __linux_timer_t *__timer_id) {
	ff_s32_t ret;
	__asm__("xorq %%rdi, %%rdi\n\t"
			"movl %1, %%edi\n\t"
			"movq %2, %%rsi\n\t"
			"movq %3, %%rdx\n\t"
			"call __timer_create\n\t"
			"movl %%eax, %0" : "=m"(ret) : "m"(__clock), "m"(__event), "m"(__timer_id) : "rdi", "rsi", "rdx", "rax"
	);

	return ret;
}
