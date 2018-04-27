# ifndef __ffly__barycentric__h
# define __ffly__barycentric__h
# include "../ffint.h"
# include "../vertex.h"
# ifdef __cplusplus
extern "C" {
# endif
void ffly_barycentric(ff_int_t, ff_int_t, ff_int_t, ffly_vertexp, ffly_vertexp, ffly_vertexp, float*, float*, float*);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__barycentric__h*/
