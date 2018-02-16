# include <mdlint.h>
void* brk(void *__p) {
    __asm__("mov %0, %%rdi\n\t"
        "call _brk" : : "m"(__p));
}
