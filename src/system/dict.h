# ifndef __ffly__system__dict__h
# define __ffly__system__dict__h
# include "../types/err_t.h"
# include "lat.h"
/*
	using this at a large scale 
	will result in large amount of memory being used.
	but lookup time will be faster then a map.
*/
typedef struct ffly_dict {
	struct ffly_lat lat;
} *ffly_dictp;

ffly_err_t ffly_dict_init(ffly_dictp);
ffly_err_t ffly_dict_put(ffly_dictp, char const*, void const*);
void const* ffly_dict_get(ffly_dictp, char const*, ffly_err_t*);
ffly_err_t ffly_dict_de_init(ffly_dictp);
# endif /*__ffly__sysem__dict__h*/
