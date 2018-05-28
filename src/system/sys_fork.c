# include "../linux/types.h"
__linux_pid_t fork() {
	__linux_pid_t ret;
	__asm__("call __fork\n\t"
			"movl %%eax, %0": "=m"(ret) : : "rax");
	return ret;
}
