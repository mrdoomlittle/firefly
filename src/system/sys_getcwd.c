# include <mdlint.h>
mdl_s32_t getcwd(char *__buf, mdl_u64_t __size) {
	mdl_s32_t ret;
	__asm__("mov %1, %%rdi\n\t"
			"mov %2, %%rsi\n\t"
			"call _getcwd\n\t"
			"mov %%eax, %0" : "=r"(ret) : "r"(__buf), "r"(__size));
	return ret;
}
