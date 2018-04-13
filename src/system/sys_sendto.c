# include "../ffint.h"
# include "../types/socket.h"
# include "../linux/socket.h"
ff_s32_t sendto(ff_u32_t __fd, void *__buf, ff_u32_t __size, ff_u32_t __flags, struct sockaddr *__adr, sockl_t __len) {
	ff_s32_t ret;
	__asm__("xorq %%rdi, %%rdi\n\t"
			"mov %1, %%edi\n\t"
			"mov %2, %%rsi\n\t"
			"xorq %%rdx, %%rdx\n\t"
			"mov %3, %%edx\n\t"
			"xorq %%r10, %%r10\n\t"
			"mov %4, %%r10d\n\t"
			"mov %5, %%r8\n\t"
//			"xorq %%r9, %%r9\n\t"
			"mov %6, %%r9\n\t"
			"call __sendto\n\t"
			"mov %%eax, %0" : "=m"(ret) : "m"(__fd), "m"(__buf), "m"(__size), "m"(__flags), "m"(__adr), "m"(__len) :
				"rdi", "rsi", "rdx", "r10", "r8", "r9", "rax");
	return ret;
}
