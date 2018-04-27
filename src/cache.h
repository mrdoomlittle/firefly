# ifndef __ffly__cache__h
# define __ffly__cache__h
# include "ffint.h"
# include "types.h"

# ifdef __cplusplus
extern "C" {
# endif
void ffly_set_cache_dir(char*);
ff_err_t ffly_cache_prepare(ff_uint_t);
ff_err_t ffly_cache_put(ff_id_t*, void*);
void* ffly_cache_get(ff_id_t, ff_err_t*);
ff_err_t ffly_cache_del(ff_id_t);
ff_err_t ffly_cache_free();
# ifdef __cplusplus
}
# endif
# endif /*__ffly__cache__h*/
