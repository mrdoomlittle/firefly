# include "../ffint.h"
# include "../linux/types.h"
ff_s32_t kill(__linux_pid_t __pid, ff_s32_t __sig) {
	ff_s32_t ret;
	__asm__("xorq %%rdi, %%rdi\n\t"
			"movl %1, %%edi\n\t"
			"movl %2, %%esi\n\t"
			"call __kill\n\t"
			"movl %%eax, %0": "=m"(ret) : "m"(__pid), "m"(__sig) : "rdi", "esi", "rax");
	return ret;
}
