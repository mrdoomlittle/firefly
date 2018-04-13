# ifndef __ffly__mat__h
# define __ffly__mat__h
# include "ffint.h"
# include "ffly_def.h"

typedef struct ffly_mat {
	char const *p, *end;
} *ffly_matp;

void ffly_matact(ffly_matp);
# endif /*__ffly__mat__h*/
