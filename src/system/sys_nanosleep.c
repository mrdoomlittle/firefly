# include "nanosleep.h"
void ffly_nanosleep(mdl_u64_t __sec, mdl_u64_t __nsec) {
    __asm__("mov %0, %%rdi\n\t"
        "mov %1, %%rsi\n\t"
        "call __ffly_nanosleep" : : "m"(__sec), "m"(__nsec) : "rdi", "rsi");
}
