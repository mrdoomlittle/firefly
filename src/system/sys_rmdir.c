# include "../ffint.h"
ff_s32_t rmdir(char const *__dir) {
	ff_s32_t ret;
    __asm__("movq %1, %%rdi\n\t"
        "call __rmdir\n\t"
		"movl %%eax, %0" : "=m"(ret) : "m"(__dir) : "rdi", "rax");
	return ret;
}
