# include "../ffint.h"
ff_s32_t open(char const *__file, ff_s32_t __flags, ff_s32_t __mode) {
    ff_s32_t ret;
    __asm__("mov %1, %%rdi\n\t"
        "mov %2, %%esi\n\t"
        "mov %3, %%edx\n\t"
        "call __open\n\t"
        "mov %%eax, %0" : "=m"(ret) : "m"(__file), "m"(__flags), "m"(__mode) : "rdi", "esi", "edx", "rax");
    return ret;
}
