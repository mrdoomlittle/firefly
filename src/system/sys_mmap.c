# include "../ffint.h"
void* mmap(void *__addr, ff_u64_t __len, ff_u64_t __prot, ff_u64_t __flags, ff_u64_t __fd, ff_u64_t __off) {
	void *ret;
	__asm__("mov %1, %%rdi\n\t"
			"mov %2, %%rsi\n\t"
			"mov %3, %%rdx\n\t"
			"mov %4, %%r10\n\t"
			"mov %5, %%r8\n\t"
			"mov %6, %%r9\n\t"
			"call __mmap\n\t"
			"mov %%rax, %0" : "=m"(ret) : "m"(__addr), "m"(__len), "m"(__prot), "m"(__flags),
				"m"(__fd), "m"(__off) : "rdi", "rsi", "rdx", "r10", "r8", "r9", "rax");	
	return ret;
}
