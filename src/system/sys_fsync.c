# include <mdlint.h>
mdl_s32_t ffly_fsync(mdl_s32_t __fd) {
	mdl_s32_t ret;
	__asm__("mov %1, %%edi\n\t"
			"call _ffly_fsync\n\t"
			"mov %%eax, %0" : "=r"(ret) : "r"(__fd));
	return ret;
}
