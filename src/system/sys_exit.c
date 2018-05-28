# include "../ffint.h"
ff_s32_t exit(ff_s32_t __exit_code) {
	ff_s32_t ret;
	__asm__("movl %1, %%edi\n\t"
			"call _exit\n\t"
			"movl %%eax, %0": "=m"(ret) : "m"(__exit_code) : "edi", "rax");
	return ret;
}
