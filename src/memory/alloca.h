# ifndef __ffly__alloca__h
# define __ffly__alloca__h
/*
    small allocations only.
    frame method.
*/
# include "../ffint.h"
# include "../types.h"
# ifdef __cplusplus
extern "C" {
# endif
void *ffly_frame();
void* ffly_alloca(ff_uint_t, ff_err_t*);
void ffly_trim(ff_uint_t);
void ffly_collapse(void*);
void ffly_alloca_cleanup();
void ffly_alss(void*, ff_uint_t);
void ffly_alsst(void*);
void ffly_alsld(void*, ff_uint_t);
ff_uint_t ffly_alssz();
void ffly_alrig();
void ffly_alrr();
void ffly_alad(void**);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__alloca__h*/
