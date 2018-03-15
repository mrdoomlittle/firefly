# include <mdlint.h>
mdl_s32_t ftruncate(mdl_u32_t __fd, mdl_u32_t __len) {
	mdl_s32_t ret;
	__asm__("xorq %%rdi, %%rdi\n\t"
			"mov %1, %%edi\n\t"
			"xorq %%rsi, %%rsi\n\t"
			"mov %2, %%esi\n\t"
			"call __ftruncate\n\t"
			"mov %%eax, %0" : "=m"(ret) : "m"(__fd), "m"(__len) : "rdi", "rsi", "rax");
	return ret;
}
