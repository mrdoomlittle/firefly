# include <mdlint.h>
void* shmat(mdl_s32_t __id, void *__addr, mdl_s32_t __flg) {
	void *ret;
	__asm__("mov %1, %%edi\n\t"
			"mov %2, %%rsi\n\t"
			"mov %3, %%edx\n\t"
			"call _shmat\n\t"
			"mov %%rax, %0" : "=m"(ret) : "m"(__id), "m"(__addr), "m"(__flg));
	return ret;
}
