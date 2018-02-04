# include <mdlint.h>
mdl_s32_t ffly_write(mdl_u32_t __fd, void *__buf, mdl_u64_t __size) {
    __asm__("mov %0, %%edi\n\t"
        "mov %1, %%rsi\n\t"
        "mov %2, %%rdx\n\t"
        "call _ffly_write" : : "r"(__fd), "r"(__buf), "r"(__size));
}
