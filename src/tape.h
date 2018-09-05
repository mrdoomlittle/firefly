# ifndef __ffly__tape__h
# define __ffly__tape__h
# include "ffint.h"

typedef struct ffly_tape {
	void *area;
	ff_uint_t len, page_c;
} *ffly_tapep;

struct ffly_tape *ffly_tape_new(void);
void ffly_tape_raze(ffly_tapep);
void ffly_tape_get(ffly_tapep, ff_uint_t, void*, ff_uint_t);
void ffly_tape_insert(ffly_tapep, void*, ff_uint_t);
# endif /*__ffly__tape__h*/
