# include "../ffint.h"
# include "../types/socket.h"
# include "../linux/socket.h"
ff_s32_t accept(ff_u32_t __fd, struct sockaddr *__adr, sockl_t *__len) {
	ff_s32_t ret;
	__asm__("xorq %%rdi, %%rdi\n\t"
			"movl %1, %%edi\n\t"
			"movq %2, %%rsi\n\t"
			"movq %3, %%rdx\n\t"
			"call __accept\n\t"
			"movl %%eax, %0" : "=m"(ret) : "m"(__fd), "m"(__adr), "m"(__len) : "rdi", "rsi", "rdx", "rax");
	return ret;
}
