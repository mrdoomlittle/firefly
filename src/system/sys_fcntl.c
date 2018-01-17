# include <mdlint.h>
mdl_int_t extern _ffly_fcntl();
mdl_int_t ffly_fcntl(mdl_int_t __fd, mdl_int_t __cmd, mdl_u64_t __arg) {
    __asm__("mov %0, %%rdi\n\t"
        "mov %1, %%rsi\n\t"
        "mov %2, %%rdx\n\t"
        "call _ffly_fcntl" : : "r"((mdl_i64_t)__fd), "r"((mdl_i64_t)__cmd), "r"(__arg));
}
