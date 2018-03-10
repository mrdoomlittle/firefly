# include <mdlint.h>
mdl_s32_t lseek(mdl_u32_t __fd, mdl_u64_t __off, mdl_u32_t __whence) {
	mdl_s32_t ret;
	__asm__("xorq %%rdi, %%rdi\n\t"
			"mov %1, %%edi\n\t"
			"mov %2, %%rsi\n\t"
			"mov %3, %%edx\n\t"
			"call _lseek\n\t"
			"mov %%eax, %0" : "=m"(ret) : "m"(__fd), "m"(__off), "m"(__whence) : "rdi", "rsi", "edx", "rax");
	return ret;
}
