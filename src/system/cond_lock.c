# include "../types/cond_lock_t.h"
void ffly_cond_lock_wait(ffly_cond_lock_t *__lock) {
	__asm__("mov %0, %%rdi\n\t"
			"call __ffly_cond_lock_wait" : : "m"(__lock) : "rdi");
}

void ffly_cond_lock_signal(ffly_cond_lock_t *__lock) {
	__asm__("mov %0, %%rdi\n\t"
			"call __ffly_cond_lock_signal" : : "m"(__lock) : "rdi");
}
