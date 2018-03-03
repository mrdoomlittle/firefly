# include <mdlint.h>
mdl_s32_t close(mdl_u32_t __fd) {
    __asm__("mov %0, %%edi\n\t"
        "call _close" : : "m"(__fd) : "edi");
}
