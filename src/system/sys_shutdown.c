# include "../ffint.h"
ff_s32_t shutdown(ff_u32_t __fd, ff_u32_t __how) {
	ff_s32_t ret;
    __asm__("xorq %%rdi, %%rdi\n\t"
		"mov %1, %%edi\n\t"
		"xorq %%rsi, %%rsi\n\t"
		"mov %2, %%esi\n\t"
        "call __shutdown\n\t"
		"mov %%eax, %0": "=m"(ret) : "m"(__fd), "m"(__how) : "rdi", "rsi", "rax");
	return ret;
}
