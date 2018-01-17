# include <mdlint.h>
mdl_int_t extern _ffly_write();
mdl_int_t ffly_write(mdl_int_t __fd, void *__buf, mdl_uint_t __size) {
    __asm__("mov %0, %%rdi\n\t"
        "mov %1, %%rsi\n\t"
        "mov %2, %%rdx\n\t"
        "call _ffly_write" : : "r"((mdl_i64_t)__fd), "r"(__buf), "r"((mdl_u64_t)__size));
}
