# include "atomic.h"
void extern _ffly_atomic_add(mdl_u64_t*, mdl_u64_t);
void extern _ffly_atomic_sub(mdl_u64_t*, mdl_u64_t);
void extern _ffly_atomic_set(mdl_u64_t*, mdl_u64_t);
mdl_u64_t extern _ffly_atomic_get(mdl_u64_t*);
void extern _ffly_atomic_incr(mdl_u64_t*);
void extern _ffly_atomic_decr(mdl_u64_t*);

void ffly_atomic_add(mdl_u64_t *__p, mdl_u64_t __no) {
    __asm__("mov %0, %%rdi\n\t"
        "mov %1, %%rsi\n\t"
        "call _ffly_atomic_add" : : "r"(__p), "r"(__no));
}

void ffly_atomic_sub(mdl_u64_t *__p, mdl_u64_t __no) {
    __asm__("mov %0, %%rdi\n\t"
        "mov %1, %%rsi\n\t"
        "call _ffly_atomic_sub" : : "r"(__p), "r"(__no));
}

void ffly_atomic_set(mdl_u64_t *__p, mdl_u64_t __to) {
    __asm__("mov %0, %%rdi\n\t"
        "mov %1, %%rsi\n\t"
        "call _ffly_atomic_set" : : "r"(__p), "r"(__to));
}

mdl_u64_t ffly_atomic_get(mdl_u64_t *__p) {
    __asm__("mov %0, %%rdi\n\t"
        "call _ffly_atomic_get" : : "r"(__p));
}

void ffly_atomic_incr(mdl_u64_t *__p) {
    __asm__("mov %0, %%rdi\n\t"
        "call _ffly_atomic_incr" : : "r"(__p));
}

void ffly_atomic_decr(mdl_u64_t *__p) {
    __asm__("mov %0, %%rdi\n\t"
        "call _ffly_atomic_decr" : : "r"(__p));
}
/*
# include <stdio.h>
int main() {
    mdl_u64_t example = 0;
    ffly_atomic_add(&example, 20);
    ffly_atomic_incr(&example);
    printf("%u\n", ffly_atomic_get(&example));
}*/
