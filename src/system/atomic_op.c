# include "atomic.h"
void ffly_atomic_add(ff_u64_t *__p, ff_u64_t __no) {
    __asm__("mov %0, %%rdi\n\t"
        "mov %1, %%rsi\n\t"
        "call __ffly_atomic_add" : : "r"(__p), "r"(__no));
}

void ffly_atomic_sub(ff_u64_t *__p, ff_u64_t __no) {
    __asm__("mov %0, %%rdi\n\t"
        "mov %1, %%rsi\n\t"
        "call __ffly_atomic_sub" : : "r"(__p), "r"(__no));
}

void ffly_atomic_set(ff_u64_t *__p, ff_u64_t __to) {
    __asm__("mov %0, %%rdi\n\t"
        "mov %1, %%rsi\n\t"
        "call __ffly_atomic_set" : : "r"(__p), "r"(__to));
}

ff_u64_t ffly_atomic_get(ff_u64_t *__p) {
    __asm__("mov %0, %%rdi\n\t"
        "call __ffly_atomic_get" : : "r"(__p));
}

void ffly_atomic_incr(ff_u64_t *__p) {
    __asm__("mov %0, %%rdi\n\t"
        "call __ffly_atomic_incr" : : "r"(__p));
}

void ffly_atomic_decr(ff_u64_t *__p) {
    __asm__("mov %0, %%rdi\n\t"
        "call __ffly_atomic_decr" : : "r"(__p));
}
/*
# include <stdio.h>
int main() {
    ff_u64_t example = 0;
    ffly_atomic_add(&example, 20);
    ffly_atomic_incr(&example);
    printf("%u\n", ffly_atomic_get(&example));
}*/
