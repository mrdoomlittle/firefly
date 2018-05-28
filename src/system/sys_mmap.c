# include "../ffint.h"
void* mmap(void *__addr, ff_u64_t __len, ff_u64_t __prot, ff_u64_t __flags, ff_u64_t __fd, ff_u64_t __off) {
	void *ret;
	__asm__("movq %1, %%rdi\n\t"
			"movq %2, %%rsi\n\t"
			"movq %3, %%rdx\n\t"
			"movq %4, %%r10\n\t"
			"movq %5, %%r8\n\t"
			"movq %6, %%r9\n\t"
			"call __mmap\n\t"
			"movq %%rax, %0" : "=m"(ret) : "m"(__addr), "m"(__len), "m"(__prot), "m"(__flags),
				"m"(__fd), "m"(__off) : "rdi", "rsi", "rdx", "r10", "r8", "r9", "rax");	
	return ret;
}
