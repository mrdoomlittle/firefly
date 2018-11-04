# include "frag.h"
# include "../malloc.h"
struct frag* bond_fnew(ff_u64_t __src, ff_uint_t __size) {
	struct frag *f;

	f = (struct frag*)malloc(sizeof(struct frag));
	f->src = __src;
	f->size = __size;
	return f;
}

void bond_fdestroy(struct frag *__f) {
	free(__f);
}
