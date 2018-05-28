# include "../ffint.h"
ff_s32_t fcntl(ff_u32_t __fd, ff_s32_t __cmd, ff_u64_t __arg) {
	ff_s32_t ret;
	__asm__("movl %1, %%edi\n\t"
        "movl %2, %%esi\n\t"
        "movq %3, %%rdx\n\t"
        "call __fcntl\n\t"
		"movl %%eax, %0" : "=m"(ret) : "m"(__fd), "m"(__cmd), "m"(__arg) : "edi", "esi", "rdx", "rax");
	return ret;
}
