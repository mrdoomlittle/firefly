# include <mdlint.h>
mdl_s32_t ffly_open(char const *__file, mdl_s32_t __flags, mdl_s32_t __mode) {
    mdl_s32_t ret;
    __asm__("mov %1, %%rdi\n\t"
        "mov %2, %%esi\n\t"
        "mov %3, %%edx\n\t"
        "call _ffly_open\n\t"
        "mov %%eax, %0" : "=r"(ret) : "r"(__file), "r"(__flags), "r"(__mode));
    return ret;
}
