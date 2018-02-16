# include "../linux/types.h"
__linux_pid_t getppid(void) {
	__linux_pid_t ret;
	__asm__("call _getppid\n\t"
			"mov %%eax, %0" : "=m"(ret) :);
	return ret;
}
