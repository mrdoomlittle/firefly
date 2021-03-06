# ifndef __ffly__tls__h
# define __ffly__tls__h
# include "../ffint.h"
# include "../types.h"
void ffly_tls_toinit(void(*)(void));
void ffly_tls_init(void);
ff_err_t ffly_tls_new(void);
void ffly_tls_destroy(void);
ff_uint_t ffly_tls_alloc(ff_uint_t);
extern ff_uint_t tls_off;
void ffly_tls_set(ff_u64_t, ff_uint_t);
ff_u64_t ffly_tls_get(ff_uint_t);
void* ffly_tls_getp(ff_uint_t);
# endif /*__ffly__tls__h*/
