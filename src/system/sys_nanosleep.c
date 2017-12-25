# include "nanosleep.h"
void extern _ffly_nanosleep(mdl_u64_t, mdl_u64_t);
void ffly_nanosleep(mdl_u64_t __sec, mdl_u64_t __nsec) {
    __asm__("mov %0, %%rdi\n\t"
        "mov %1, %%rsi\n\t"
        "call _ffly_nanosleep" : : "r"(__sec), "r"(__nsec));
}
/*
int main() {
    ffly_nanosleep(0, 100000000);
}*/
