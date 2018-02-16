# include <mdlint.h>
mdl_s32_t fsync(mdl_s32_t __fd) {
	mdl_s32_t ret;
	__asm__("mov %1, %%edi\n\t"
			"call _fsync\n\t"
			"mov %%eax, %0" : "=m"(ret) : "m"(__fd));
	return ret;
}
