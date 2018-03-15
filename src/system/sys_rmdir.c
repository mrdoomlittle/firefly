# include <mdlint.h>
mdl_s32_t rmdir(char const *__dir) {
	mdl_s32_t ret;
    __asm__("mov %1, %%rdi\n\t"
        "call __rmdir\n\t"
		"mov %%eax, %0" : "=m"(ret) : "m"(__dir) : "rdi", "rax");
	return ret;
}
