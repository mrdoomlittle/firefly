# include <mdlint.h>
void extern* _ffly_munmap();
void ffly_munmap(void *__addr, mdl_u64_t __len) {
	__asm__("mov %0, %%rdi\n\t"
			"mov %1, %%rsi\n\t"
			"call _ffly_mmap" : : "r"(__addr), "r"(__len));	
}
