# include "../linux/types.h"
__linux_pid_t fork() {
	__linux_pid_t ret;
	__asm__("call _fork\n\t"
			"mov %%eax, %0": "=m"(ret) : : "rax");
	return ret;
}
