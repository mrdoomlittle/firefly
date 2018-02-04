# include "../linux/types.h"
__linux_pid_t ffly_getpid(void) {
	__linux_pid_t ret;
	__asm__("call _ffly_getpid\n\t"
			"mov %%eax, %0" : "=r"(ret) :);
	return ret;
}
