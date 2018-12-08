# include "frag.h"
# include "../malloc.h"
# include "../stdio.h"
struct frag static **frags = NULL;
ff_uint_t static nf = 0;

struct frag *bond_fbn(ff_uint_t __n) {
	return *(frags+__n);
}

struct frag *fr_head = NULL;
struct frag static *curfrag = NULL;
struct frag* bond_fnew(ff_u64_t __adr, ff_uint_t __size, ff_uint_t __n) {
	ff_uint_t start;
	start = nf;
	nf = __n+1;
	struct frag *f;
	if (!frags) {
		frags = (struct frag**)malloc(nf*sizeof(struct frag*));
	} else {
		if (__n>=start) {
			frags = (struct frag**)realloc(frags, nf*sizeof(struct frag*));			
		}
		goto _sk;
	}

	ff_uint_t i;
	i = start;
	for(;i != nf;i++) {
		*(frags+i) = NULL;
	}
_sk:
	f = (struct frag*)malloc(sizeof(struct frag));
	f->adr = __adr;
	f->size = __size;
	f->f = __n;
	*(frags+__n) = f;
	f->bs = 0;
	f->m = 0;
	if (!fr_head)
		fr_head = f;
	if (curfrag != NULL)
		curfrag->next = f;
	curfrag = f;
	f->next = NULL;
	printf("frag: adr: %u, sz: %u, id: %u\n", __adr, __size, __n);
	return f;
}

void bond_fdestroy(struct frag *__f) {
	free(__f);
}
