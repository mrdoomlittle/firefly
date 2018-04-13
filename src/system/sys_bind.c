# include "../ffint.h"
# include "../types/socket.h"
# include "../linux/socket.h"
ff_s32_t bind(ff_u32_t __fd, struct sockaddr *__adr, sockl_t __len) {
	ff_s32_t ret;
	__asm__("xorq %%rdi, %%rdi\n\t"
			"mov %1, %%edi\n\t"
			"mov %2, %%rsi\n\t"
			"xorq %%rdx, %%rdx\n\t"
			"mov %3, %%edx\n\t"
			"call __bind\n\t"
			"mov %%eax, %0" : "=m"(ret) : "m"(__fd), "m"(__adr), "m"(__len) : "rdi", "rsi", "rdx", "rax");
	return ret;
}
