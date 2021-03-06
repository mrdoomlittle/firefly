# include "../ffint.h"
# include "../linux/types.h"
# include "../linux/resource.h"
ff_s32_t wait4(__linux_pid_t __pid, int *__stat_adr, int __opt, struct rusage *__ru) {
	ff_s32_t ret;
	__asm__("xorq %%rdi, %%rdi\n\t"
			"movl %1, %%edi\n\t"
			"movq %2, %%rsi\n\t"
			"movq %3, %%rdx\n\t"
			"movq %4, %%r10\n\t"
			"call __wait4\n\t"
			"movl %%eax, %0": "=m"(ret) : "m"(__pid), "m"(__stat_adr), "m"(__opt), "m"(__ru) : "rdi", "rsi", "rdx", "r10", "rax");
	return ret;
}
