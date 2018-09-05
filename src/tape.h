# ifndef __ffly__tape__h
# define __ffly__tape__h
# include "ffint.h"

struct ffly_tape {
	void *area;
	ff_uint_t len, page_c;
};

struct ffly_tape *ffly_tape_new(void);
void ffly_tape_raze(struct ffly_tape*);
void ffly_tape_insert(struct ffly_tape*, void*, ff_uint_t);
# endif /*__ffly__tape__h*/
