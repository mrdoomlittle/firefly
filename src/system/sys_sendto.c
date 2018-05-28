# include "../ffint.h"
# include "../types/socket.h"
# include "../linux/socket.h"
ff_s32_t sendto(ff_u32_t __fd, void *__buf, ff_u32_t __size, ff_u32_t __flags, struct sockaddr *__adr, sockl_t __len) {
	ff_s32_t ret;
	__asm__("xorq %%rdi, %%rdi\n\t"
			"movl %1, %%edi\n\t"
			"movq %2, %%rsi\n\t"
			"xorq %%rdx, %%rdx\n\t"
			"movl %3, %%edx\n\t"
			"xorq %%r10, %%r10\n\t"
			"movl %4, %%r10d\n\t"
			"movq %5, %%r8\n\t"
//			"xorq %%r9, %%r9\n\t"
			"movq %6, %%r9\n\t"
			"call __sendto\n\t"
			"movl %%eax, %0" : "=m"(ret) : "m"(__fd), "m"(__buf), "m"(__size), "m"(__flags), "m"(__adr), "m"(__len) :
				"rdi", "rsi", "rdx", "r10", "r8", "r9", "rax");
	return ret;
}
