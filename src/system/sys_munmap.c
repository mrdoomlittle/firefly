# include <mdlint.h>
void munmap(void *__addr, mdl_u64_t __len) {
	__asm__("mov %0, %%rdi\n\t"
			"mov %1, %%rsi\n\t"
			"call _munmap" : : "m"(__addr), "m"(__len));	
}
