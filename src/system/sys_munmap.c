# include <mdlint.h>
mdl_s32_t munmap(void *__addr, mdl_u64_t __len) {
	mdl_s32_t ret;
	__asm__("mov %1, %%rdi\n\t"
			"mov %2, %%rsi\n\t"
			"call _munmap\n\t"
			"mov %%eax, %0": "=m"(ret) : "m"(__addr), "m"(__len));	
	return ret;
}
