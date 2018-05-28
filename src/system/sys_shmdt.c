# include "../ffint.h"
ff_s32_t shmdt(void *__addr) {
	ff_s32_t ret;
	__asm__("movq %1, %%rdi\n\t"
			"call __shmdt\n\t"
			"movl %%eax, %0" : "=m"(ret) : "m"(__addr) : "rdi", "rax");
	return ret;
}
