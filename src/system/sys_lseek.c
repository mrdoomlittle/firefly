# include <mdlint.h>
mdl_s32_t ffly_lseek(mdl_u32_t __fd, mdl_u64_t __off, mdl_u32_t __whence) {
	mdl_s32_t ret;
	__asm__("mov %1, %%edi\n\t"
			"mov %2, %%rsi\n\t"
			"mov %3, %%edx\n\t"
			"call _ffly_lseek\n\t"
			"mov %%eax, %0" : "=r"(ret) : "r"(__fd), "r"(__off), "r"(__whence));
	return ret;
}
