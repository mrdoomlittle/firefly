# include <mdlint.h>
mdl_int_t ffly_close(mdl_int_t __fd) {
    __asm__("mov %0, %%rdi\n\t"
        "call _ffly_close" : : "r"((mdl_i64_t)__fd));
}
