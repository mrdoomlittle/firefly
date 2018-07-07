# include "signal.h"
void __restore();
__asm__("__restore:\n\t"
		"movq $15,%rax\n\t"
		"syscall");

# include "dep/mem_cpy.h"
ff_s32_t sigaction(ff_s32_t __sig, struct sigaction const *__act, struct sigaction *__oact) {
	struct sigaction sa;
	ffly_mem_cpy(&sa, __act, sizeof(struct sigaction));
	sa.sa_restorer = __restore;
	sa.sa_flags |= SA_RESTORER;
	return rt_sigaction(__sig, &sa, __oact, sizeof(sigset_t));
}
