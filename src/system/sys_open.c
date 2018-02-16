# include <mdlint.h>
mdl_s32_t open(char const *__file, mdl_s32_t __flags, mdl_s32_t __mode) {
    mdl_s32_t ret;
    __asm__("mov %1, %%rdi\n\t"
        "mov %2, %%esi\n\t"
        "mov %3, %%edx\n\t"
        "call _open\n\t"
        "mov %%eax, %0" : "=m"(ret) : "m"(__file), "m"(__flags), "m"(__mode));
    return ret;
}
