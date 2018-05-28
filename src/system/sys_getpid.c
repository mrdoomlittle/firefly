# include "../linux/types.h"
__linux_pid_t getpid(void) {
	__linux_pid_t ret;
	__asm__("call __getpid\n\t"
			"movl %%eax, %0" : "=m"(ret) : : "rax");
	return ret;
}
