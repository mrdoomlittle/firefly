# include "slurry.h"
# ifndef __fflib
# include <malloc.h>
# include <sys/socket.h>
# else
# include "../malloc.h"
# include "../linux/socket.h"
# endif
void *s_ov[] = {
	NULL
};

void static
_send(void *__buf, ff_uint_t __size, long long __sock) {
	send(__sock, __buf, __size, 0);
}

void static
_recv(void *__buf, ff_uint_t __size, long long __sock) {
	recv(__sock, __buf, __size, 0);
}

void(*s_io_send)(void*, ff_uint_t, long long) = _send;
void(*s_io_recv)(void*, ff_uint_t, long long) = _recv;

s_tapep s_tape_new(ff_uint_t __sz) {
	s_tapep t;
	t = (s_tapep)malloc(sizeof(struct s_tape));
	t->text = malloc(__sz);
	t->len = __sz;
	return t;
}

void s_tape_destroy(s_tapep __t) {
	free(__t->text);
	free(__t);
}
