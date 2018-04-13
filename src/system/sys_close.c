# include "../ffint.h"
ff_s32_t close(ff_u32_t __fd) {
    __asm__("xorq %%rdi, %%rdi\n\t"
		"mov %0, %%edi\n\t"
        "call _close" : : "m"(__fd) : "rdi");
}
