# include <mdlint.h>
# include "../types/socket.h"
# include "../linux/socket.h"
mdl_s32_t accept(mdl_u32_t __fd, struct sockaddr *__adr, sockl_t *__len) {
	mdl_s32_t ret;
	__asm__("xorq %%rdi, %%rdi\n\t"
			"mov %1, %%edi\n\t"
			"mov %2, %%rsi\n\t"
			"mov %3, %%rdx\n\t"
			"call __accept\n\t"
			"mov %%eax, %0" : "=m"(ret) : "m"(__fd), "m"(__adr), "m"(__len) : "rdi", "rsi", "rdx", "rax");
	return ret;
}
