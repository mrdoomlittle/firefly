# include <mdlint.h>
mdl_s32_t ffly_close(mdl_u32_t __fd) {
    __asm__("mov %0, %%edi\n\t"
        "call _ffly_close" : : "r"(__fd));
}
