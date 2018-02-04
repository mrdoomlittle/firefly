# include <mdlint.h>
void extern* _ffly_mmap();
void* ffly_mmap(void *__addr, mdl_u64_t __len, mdl_u64_t __prot, mdl_u64_t __flags, mdl_u64_t __fd, mdl_u64_t __off) {
	void *ret;
	__asm__("mov %1, %%rdi\n\t"
			"mov %2, %%rsi\n\t"
			"mov %3, %%rdx\n\t"
			"mov %4, %%r10\n\t"
			"mov %5, %%r8\n\t"
			"mov %6, %%r9\n\t"
			"call _ffly_mmap\n\t"
			"mov %%rax, %0" : "=r"(ret) : "r"(__addr), "r"(__len), "r"(__prot), "r"(__flags), "r"(__fd), "r"(__off));	
	return ret;
}
