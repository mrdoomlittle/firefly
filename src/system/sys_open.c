# include "../ffint.h"
ff_s32_t open(char const *__file, ff_s32_t __flags, ff_s32_t __mode) {
    ff_s32_t ret;
    __asm__("movq %1, %%rdi\n\t"
        "movl %2, %%esi\n\t"
        "movl %3, %%edx\n\t"
        "call __open\n\t"
        "movl %%eax, %0" : "=m"(ret) : "m"(__file), "m"(__flags), "m"(__mode) : "rdi", "esi", "edx", "rax");
    return ret;
}
