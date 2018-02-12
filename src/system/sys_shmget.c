# include "../linux/types.h"
# include <mdlint.h>
mdl_s32_t shmget(__linux_key_t __key, __linux_size_t __size, mdl_s32_t __flags) {
	mdl_s32_t ret;
	__asm__("mov %1, %%edi\n\t"
			"mov %2, %%rsi\n\t"
			"mov %3, %%edx\n\t"
			"call _shmget\n\t"
			"mov %%eax, %0" : "=r"(ret) : "r"(__key), "r"(__size), "r"(__flags));
	return ret;
}
