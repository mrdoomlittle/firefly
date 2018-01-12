# ifndef __ffly__cache__h
# define __ffly__cache__h
# include "types/err_t.h"
# include "types/id_t.h"

# ifdef __cplusplus
extern "C" {
# endif
void ffly_set_cache_dir(char*);
ffly_err_t ffly_cache_prepare(mdl_uint_t);
ffly_err_t ffly_cache_put(ffly_id_t*, void*);
void* ffly_cache_get(ffly_id_t, ffly_err_t*);
ffly_err_t ffly_cache_del(ffly_id_t);
ffly_err_t ffly_cache_free();
# ifdef __cplusplus
}
# endif
# endif /*__ffly__cache__h*/
