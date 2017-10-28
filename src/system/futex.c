# define _GNU_SOURCE
# include "futex.h"
# include <stddef.h>
# include <sys/syscall.h>
# include <linux/futex.h>
# include <unistd.h>
void static fwait(int *__f) {
	_again:
	if (__sync_bool_compare_and_swap(__f, 1, 0)) return;
	syscall(SYS_futex, __f, FUTEX_WAIT, 0, NULL, NULL, 0);
	goto _again;
}

void static fwake(int *__f) {
	if (!__sync_bool_compare_and_swap(__f, 0, 1)) return;
	syscall(SYS_futex, __f, FUTEX_WAKE, 1, NULL, NULL, 0);
}

int static f1 = 0, f2 = 1;
void _flock() {
	flock(&f1, &f2);
}

void _funlock() {
	funlock(&f1, &f2);
}

void flock(int *__f1, int *__f2) {
	if (!*__f1)
		fwait(__f2);
	else
		fwait(__f1);
}

void funlock(int *__f1, int *__f2) {
	if (*__f2)
		fwake(__f1);
	else
		fwake(__f2);
}
