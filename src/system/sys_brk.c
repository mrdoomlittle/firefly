# include <mdlint.h>
void* brk(void *__p) {
	void *ret;
    __asm__("mov %1, %%rdi\n\t"
        "call __brk\n\t"
		"mov %%rax, %0": "=m"(ret) : "m"(__p) : "rdi", "rax");
	return ret;
}
