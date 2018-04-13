# include "mutex.h"
void ffly_mutex_lock(ff_mlock_t *__lock) {
	__asm__("mov %0, %%rdi\n\t"
        "call __ffly_mutex_lock" : : "r"(__lock));
}

void ffly_mutex_unlock(ff_mlock_t *__lock) {
    __asm__("mov %0, %%rdi\n\t" 
        "call __ffly_mutex_unlock" : : "r"(__lock));
}

ff_err_t ffly_mutex_trylock(ff_mlock_t *__lock) {
    ff_err_t ret;
    __asm__("mov %1, %%rdi\n\r"
        "call __ffly_mutex_trylock\n\t" 
        "mov %%bl, %0" : "=r"(ret) : "r"(__lock));
    return ret;
}

/*
# include <pthread.h>
# include <stdio.h>
ffly_mutex_t m = FFLY_MUTEX_INIT;
mdl_uint_t x = 0;
void* test(void* __arg) {
    mdl_uint_t i = 0;
    while(i++ != 20000) {
    ffly_mutex_lock(&m);
        x++;
    ffly_mutex_unlock(&m);
    }
}

int main() {
    pthread_t a,b;
    pthread_create(&a, NULL, test, NULL);
    pthread_create(&b, NULL, test, NULL);
    pthread_join(a, NULL);
    pthread_join(b, NULL);
    printf("%u\n", x);
    //ffly_mutex_lock(&m);
  //  ffly_mutex_lock(&m);
//    printf("%u\n", m);
}*/
