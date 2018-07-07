# include "../dep/mem_set.h"
# include "../linux/signal.h"
void sigemptyset(sigset_t *__s) {
	ffly_mem_set(__s, 0, sizeof(sigset_t));
}
