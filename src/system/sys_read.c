# include <mdlint.h>
# include "io.h"
mdl_s32_t read(mdl_u32_t __fd, void *__buf, mdl_u64_t __size) {
	mdl_u32_t ret;
    __asm__("mov %1, %%edi\n\t"
        "mov %2, %%rsi\n\t"
        "mov %3, %%rdx\n\t"
        "call _read\n\t"
		"mov %%eax, %0": "=m"(ret) : "m"(__fd), "m"(__buf), "m"(__size));
	return ret;
}
