# ifndef __ffly__cistern__h
# define __ffly__cistern__h
# include "../ffint.h"
/*
	diff from resv

	- more personalized idk its because resv is being used 
	for game gravity shit and other higher level crap.
*/
typedef struct ffly_cis_slab {
	ff_u64_t off;
	struct ffly_cis_slab **bk, *fd;
	struct ffly_cis_slab *prev, *next;
	ff_i8_t inuse;
} *ffly_cis_slabp;

typedef struct ffly_cistern {
	int fd;
	ff_u64_t off;
	ffly_cis_slabp bin, top;

} *ffly_cisternp;

void ffly_cistern_init(ffly_cisternp, char const*);
void ffly_cistern_de_init(ffly_cisternp);
void* ffly_cistern_alloc(ffly_cisternp, ff_uint_t);
void ffly_cistern_free(ffly_cisternp, void*);

void ffly_cistern_write(ffly_cisternp, void*, void*, ff_uint_t, ff_uint_t);
void ffly_cistern_read(ffly_cisternp, void*, void*, ff_uint_t, ff_uint_t);
# endif /*__ffly__cistern__h*/
