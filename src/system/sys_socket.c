# include <mdlint.h>
mdl_s32_t socket(mdl_u32_t __family, mdl_u32_t __type, mdl_u32_t __proto) {
	mdl_s32_t ret;
	__asm__("xorq %%rdi, %%rdi\n\t"
			"mov %1, %%edi\n\t"
			"xorq %%rsi, %%rsi\n\t"
			"mov %2, %%esi\n\t"
			"xorq %%rdx, %%rdx\n\t"
			"mov %3, %%edx\n\t"
			"call __socket\n\t"
			"mov %%eax, %0" : "=m"(ret) : "m"(__family), "m"(__type), "m"(__proto) : "rdi", "rsi", "rdx", "rax");
	return ret;
}
