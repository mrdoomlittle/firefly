# include "../ffint.h"
ff_s32_t execve(char const *__file, char *const *__argv, char *const *__envp) {
    ff_s32_t ret;
    __asm__("movq %1, %%rdi\n\t"
        "movq %2, %%rsi\n\t"
        "movq %3, %%rdx\n\t"
        "call __execve\n\t"
        "movl %%eax, %0" : "=m"(ret) : "m"(__file), "m"(__argv), "m"(__envp) : "rdi", "rsi", "rdx", "rax");
    return ret;
}
