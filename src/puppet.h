# ifndef __ffly__puppet__h
# define __ffly__puppet__h
# include "ffint.h"
/*
	movement control will be puppeted by external entities in some cases
*/
typedef struct ffly_puppet {
	ff_uint_t *x, *y, *z;
} *ffly_puppetp;

# endif /*__ffly__puppet__h*/
