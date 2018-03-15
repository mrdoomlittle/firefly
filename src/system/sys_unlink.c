# include <mdlint.h>
mdl_s32_t unlink(char const *__path) {
	mdl_s32_t ret;
    __asm__("mov %1, %%rdi\n\t"
        "call __unlink\n\t"
		"mov %%eax, %0": "=m"(ret) : "m"(__path) : "rdi", "rax");
	return ret;
}