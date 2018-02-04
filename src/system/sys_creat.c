# include <mdlint.h>
mdl_s32_t ffly_creat(char const *__path, mdl_s32_t __mode) {
	mdl_s32_t ret;
	__asm__("mov %1, %%rdi\n\t"
			"mov %2, %%esi\n\t"
			"call _ffly_creat\n\t"
			"mov %%eax, %0" : "=r"(ret) : "r"(__path), "r"(__mode));
	return ret;
}
