# ifndef __ffly__log__h
# define __ffly__log__h
# include "io.h"
# define log(...) \
	ffly_fprintf(ffly_log, __VA_ARGS__)
# endif /*__ffly__log__h*/
