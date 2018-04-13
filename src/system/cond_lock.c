# include "../types.h"
void ffly_cond_lock_wait(ff_cond_lock_t *__lock) {
	__asm__("mov %0, %%rdi\n\t"
			"call __ffly_cond_lock_wait" : : "m"(__lock) : "rdi");
}

void ffly_cond_lock_signal(ff_cond_lock_t *__lock) {
	__asm__("mov %0, %%rdi\n\t"
			"call __ffly_cond_lock_signal" : : "m"(__lock) : "rdi");
}
