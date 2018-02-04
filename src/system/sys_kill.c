# include "../linux/types.h"
int ffly_kill(__linux_pid_t __pid, int __sig) {
	int ret;
	__asm__("mov %1, %%rdi\n\t"
			"mov %2, %%rsi\n\t"
			"call _ffly_kill\n\t"
			"mov %%eax, %0": "=r"(ret) : "r"((mdl_u64_t)__pid), "r"((mdl_u64_t)__sig));
	return ret;
}
