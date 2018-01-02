# ifndef __ffly__temp__h
# define __ffly__temp__h
/*
    small allocations only.
    frame method.
*/
# include "../types/err_t.h"
# ifdef __cplusplus
extern "C" {
# endif
void* ffly_temp_alloc(mdl_uint_t, ffly_err_t*);
ffly_err_t ffly_temp_free(void*);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__temp__h*/
