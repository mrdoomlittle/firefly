# include <mdlint.h>
void* shmat(mdl_s32_t __id, void *__addr, mdl_s32_t __flg) {
	void *ret;
	__asm__("xorq %%rdi, %%rdi\n\t"
			"mov %1, %%edi\n\t"
			"mov %2, %%rsi\n\t"
			"xorq %%rdx, %%rdx\n\t"
			"mov %3, %%edx\n\t"
			"call __shmat\n\t"
			"mov %%rax, %0" : "=m"(ret) : "m"(__id), "m"(__addr), "m"(__flg) : "rdi", "rsi", "rdx", "rax");
	return ret;
}
