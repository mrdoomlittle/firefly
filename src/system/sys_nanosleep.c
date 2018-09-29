# include "../ffint.h"
/*
	TODO:
		remove and use timespec
*/
ff_s32_t ffly_nanosleep(ff_u64_t __sec, ff_u64_t __nsec) {
	ff_s32_t ret;
	__asm__("movq %1, %%rdi\n\t"
        "movq %2, %%rsi\n\t"
        "call __ffly_nanosleep\n\t"
		"movl %%eax, %0" : "=m"(ret) : "m"(__sec), "m"(__nsec) : "rax", "rdi", "rsi");
	return ret;
}
