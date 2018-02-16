# include <mdlint.h>
mdl_s32_t access(char const *__file, mdl_u32_t __mode) {
	mdl_s32_t ret;
	__asm__("mov %1, %%rdi\n\t"
			"mov %2, %%esi\n\t" 
			"call _access\n\t"
			"mov %%eax, %0" : "=m"(ret) : "m"(__file), "m"(__mode));
	return ret;
}
