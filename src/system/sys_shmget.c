# include "../ffint.h"
# include "../linux/types.h"
ff_s32_t shmget(__linux_key_t __key, __linux_size_t __size, ff_s32_t __flags) {
	ff_s32_t ret;
	__asm__("xorq %%rdi, %%rdi\n\t"
			"mov %1, %%edi\n\t"
			"mov %2, %%rsi\n\t"
			"xorq %%rdx, %%rdx\n\t"
			"mov %3, %%edx\n\t"
			"call __shmget\n\t"
			"mov %%eax, %0" : "=m"(ret) : "m"(__key), "m"(__size), "m"(__flags) : "rdi", "rsi", "rdx", "rax");
	return ret;
}
