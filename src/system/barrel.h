# ifndef __ffly__barrel__h
# define __ffly__barrel__h
# include "../ffint.h"
# include "../types.h"
# define FFLY_BARREL_SIZE 40

# define ffly_barrel_link(__barrel, __to) \
	(__barrel)->link = __to
typedef struct ffly_barrel {
	void **blocks;
	void **fresh;
	void **unused;
	void **next;
	ff_uint_t blksize;
	struct ffly_barrel *link;
} *ffly_barrelp;

void ffly_barrel_init(ffly_barrelp, ff_uint_t);
void ffly_barrel_de_init(ffly_barrelp);
ff_bool_t ffly_barrel_full(ffly_barrelp);

void* ffly_barrel_alloc(ffly_barrelp);
void ffly_barrel_free(ffly_barrelp, void*);
# endif /*__ffly__barrel__h*/
