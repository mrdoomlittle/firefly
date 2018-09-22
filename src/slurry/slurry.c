# include "slurry.h"
# include <malloc.h>
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
