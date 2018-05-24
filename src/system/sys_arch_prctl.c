# include "../ffint.h"
ff_s32_t arch_prctl(ff_u32_t __opt, ff_u64_t __arg) {
	ff_s32_t ret;
    __asm__("xorq %%rdi, %%rdi\n\t"
			"movl %1, %%edi\n\t"
			"movq %2, %%rsi\n\t"
			"call __arch_prctl\n\t"
			"movl %%eax, %0": "=m"(ret) : "m"(__opt), "m"(__arg) : "rdi", "rsi", "rax");
	return ret;
}
