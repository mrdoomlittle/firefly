# include <mdlint.h>
mdl_s32_t creat(char const *__path, mdl_s32_t __mode) {
	mdl_s32_t ret;
	__asm__("mov %1, %%rdi\n\t"
			"mov %2, %%esi\n\t"
			"call _creat\n\t"
			"mov %%eax, %0" : "=m"(ret) : "m"(__path), "m"(__mode) : "rdi", "esi", "rax");
	return ret;
}
