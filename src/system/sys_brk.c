# include <mdlint.h>
void* ffly_brk(void *__p) {
    __asm__("mov %0, %%rdi\n\t"
        "call _ffly_brk" : : "r"(__p));
}
