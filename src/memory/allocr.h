# ifndef __ffly__allocr__h
# define __ffly__allocr__h
# include <mdlint.h>
# include "../types/err_t.h"
ffly_err_t ffly_ar_cleanup();
void* ffly_alloc(mdl_uint_t);
void ffly_free(void*);
void* ffly_realloc(void*, mdl_uint_t);
# endif /*__ffly__allocr__h*/
