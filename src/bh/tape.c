# include "../bh.h"
# include "../malloc.h"
bh_tapep bh_tape_new(ff_uint_t __sz) {
	bh_tapep t;
	t = (bh_tapep)malloc(sizeof(struct bh_tape));
	t->text = malloc(__sz);
	t->len = __sz;
	return t;
}

void bh_tape_destroy(bh_tapep __t) {
	free(__t->text);
	free(__t);
}
