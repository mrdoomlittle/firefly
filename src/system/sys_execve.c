# include <mdlint.h>
mdl_s32_t execve(char const *__file, char *const __argv[], char *const __envp[]) {
    mdl_s32_t ret;
    __asm__("mov %1, %%rdi\n\t"
        "mov %2, %%rsi\n\t"
        "mov %3, %%rdx\n\t"
        "call __execve\n\t"
        "mov %%eax, %0" : "=m"(ret) : "m"(__file), "m"(__argv), "m"(__envp) : "rdi", "rsi", "rdx", "rax");
    return ret;
}
