# ifndef __ffly__alloca__h
# define __ffly__alloca__h
/*
    small allocations only.
    frame method.
*/
# include "../types/err_t.h"
# include <mdlint.h>
# ifdef __cplusplus
extern "C" {
# endif
void *ffly_frame();
void* ffly_alloca(mdl_uint_t, ffly_err_t*);
void ffly_trim(mdl_uint_t);
void ffly_collapse(void*);
void ffly_alloca_cleanup();
void ffly_alss(void*, mdl_uint_t);
void ffly_alsst(void*);
void ffly_alsld(void*, mdl_uint_t);
mdl_uint_t ffly_alssz();
void ffly_alrig();
void ffly_alrr();
void ffly_alad(void**);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__alloca__h*/
