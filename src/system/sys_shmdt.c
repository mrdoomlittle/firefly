# include <mdlint.h>
mdl_s32_t shmdt(void *__addr) {
	mdl_s32_t ret;
	__asm__("mov %1, %%rdi\n\t"
			"call _shmdt\n\t"
			"mov %%eax, %0" : "=m"(ret) : "m"(__addr));
	return ret;
}
