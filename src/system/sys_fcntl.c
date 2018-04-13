# include "../ffint.h"
ff_s32_t fcntl(ff_u32_t __fd, ff_s32_t __cmd, ff_u64_t __arg) {
	ff_s32_t ret;
	__asm__("mov %1, %%edi\n\t"
        "mov %2, %%esi\n\t"
        "mov %3, %%rdx\n\t"
        "call __fcntl\n\t"
		"mov %%eax, %0" : "=m"(ret) : "m"(__fd), "m"(__cmd), "m"(__arg) : "edi", "esi", "rdx", "rax");
	return ret;
}
