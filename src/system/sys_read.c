# include <mdlint.h>
mdl_s32_t read(mdl_u32_t __fd, void *__buf, mdl_u64_t __size) {
    __asm__("mov %0, %%edi\n\t"
        "mov %1, %%rsi\n\t"
        "mov %2, %%rdx\n\t"
        "call _read" : : "r"(__fd), "r"(__buf), "r"(__size));
}
