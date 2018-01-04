# ifndef __ffly__chunk__h
# define __ffly__chunk__h
# include <mdlint.h>
# include "obj.h"
struct ffly_chunk {
    ffly_objpp objs;
    mdl_uint_t xal, yal, zal;
};

typedef struct ffly_chunk* ffly_chunkp;

ffly_chunkp ffly_alloc_chunk(mdl_uint_t, mdl_uint_t, mdl_uint_t);
void ffly_free_chunk(ffly_chunkp);
# endif /*__ffly__chunk__h*/
