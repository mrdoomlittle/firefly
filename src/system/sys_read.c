# include "../ffint.h"
ff_s32_t read(ff_u32_t __fd, void *__buf, ff_u64_t __size) {
	ff_u32_t ret;
    __asm__("xorq %%rdi, %%rdi\n\t"
		"mov %1, %%edi\n\t"
        "mov %2, %%rsi\n\t"
        "mov %3, %%rdx\n\t"
        "call __read\n\t"
		"mov %%eax, %0": "=m"(ret) : "m"(__fd), "m"(__buf), "m"(__size) : "rdi", "rsi", "rdx", "rax");
	return ret;
}
