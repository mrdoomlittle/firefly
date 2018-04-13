# include "../ffint.h"
ff_s32_t munmap(void *__addr, ff_u64_t __len) {
	ff_s32_t ret;
	__asm__("mov %1, %%rdi\n\t"
			"mov %2, %%rsi\n\t"
			"call __munmap\n\t"
			"mov %%eax, %0": "=m"(ret) : "m"(__addr), "m"(__len) : "rdi", "rsi", "rax");	
	return ret;
}
