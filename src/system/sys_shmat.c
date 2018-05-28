# include "../ffint.h"
void* shmat(ff_s32_t __id, void *__addr, ff_s32_t __flg) {
	void *ret;
	__asm__("xorq %%rdi, %%rdi\n\t"
			"movl %1, %%edi\n\t"
			"movq %2, %%rsi\n\t"
			"xorq %%rdx, %%rdx\n\t"
			"movl %3, %%edx\n\t"
			"call __shmat\n\t"
			"movq %%rax, %0" : "=m"(ret) : "m"(__id), "m"(__addr), "m"(__flg) : "rdi", "rsi", "rdx", "rax");
	return ret;
}
