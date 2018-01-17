# include <mdlint.h>
mdl_int_t extern _ffly_open();
mdl_int_t ffly_open(char const *__file, mdl_int_t __flags, mdl_int_t __mode) {
    mdl_i64_t ret;
    __asm__("mov %1, %%rdi\n\t"
        "mov %2, %%rsi\n\t"
        "mov %3, %%rdx\n\t"
        "call _ffly_open\n\t"
        "mov %%rax, %0" : "=r"(ret) : "r"(__file), "r"((mdl_i64_t)__flags), "r"((mdl_i64_t)__mode));
    return ret;
}
