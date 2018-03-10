# include <mdlint.h>
mdl_s32_t write(mdl_u32_t __fd, void *__buf, mdl_u64_t __size) {
	mdl_s32_t ret;
	__asm__("xorq %%rax, %%rax\n\t"
		"mov %1, %%edi\n\t"
        "mov %2, %%rsi\n\t"
        "mov %3, %%rdx\n\t"
        "call __write\n\t"
		"mov %%eax, %0" : "=m"(ret) : "m"(__fd), "m"(__buf), "m"(__size) : "rdi", "rsi", "rdx", "rax");
	return ret;
}
