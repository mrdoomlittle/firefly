# include "../ffint.h"
# include "../linux/time.h"
ff_s32_t clock_gettime(__linux_clockid_t __clock, struct timespec *__tp) {
	ff_s32_t ret;
	__asm__("xorq %%rdi, %%rdi\n\t"
			"movl %1, %%edi\n\t"
			"movq %2, %%rsi\n\t"
			"call __clock_gettime\n\t"
			"movl %%eax, %0" : "=m"(ret) : "m"(__clock), "m"(__tp) : "rdi", "rsi", "rax");
	return ret;
}
