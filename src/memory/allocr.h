# ifndef __ffly__allocr__h
# define __ffly__allocr__h
# include <mdlint.h>
# include "../types/err_t.h"

ffly_err_t ffly_ar_init();
ffly_err_t ffly_ar_cleanup();
void ffly_arstat();
void* ffly_alloc(mdl_uint_t);
void ffly_free(void*);
void* ffly_realloc(void*, mdl_uint_t);
/*should only be used for trimming small amout off*/
void* ffly_arsh(void*, mdl_uint_t);
void* ffly_argr(void*, mdl_uint_t);
void ffly_arbl(void*);
void ffly_araxe();
# endif /*__ffly__allocr__h*/
