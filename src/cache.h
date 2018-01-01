# ifndef __ffly__cache__h
# define __ffly__cache__h
# include "types/err_t.h"
# include "types/no_t.h"

void ffly_set_cache_dir(char*);
ffly_err_t ffly_cache_prepare(mdl_uint_t);
ffly_err_t ffly_cache_put(ffly_no_t*, void*);
void* ffly_cache_get(ffly_no_t, ffly_err_t*);
ffly_err_t ffly_cache_del(ffly_no_t);
ffly_err_t ffly_cache_free();
# endif /*__ffly__cache__h
