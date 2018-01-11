# ifndef __ffly__chunk__h
# define __ffly__chunk__h
# include <mdlint.h>
# include "lot.h"
# include "types/no_t.h"
# define lotsplice(__al, __lotsize) \
    (__al>>__lotsize)

struct ffly_chunk {
    mdl_u8_t lotsize;
    ffly_lotpp lots;
    mdl_uint_t lotx, loty, lotz;
    mdl_uint_t xl, yl, zl;
    mdl_uint_t x, y, z;
    ffly_no_t no;

    // cleanup
    struct ffly_chunk *prev, *next;
};

typedef struct ffly_chunk* ffly_chunkp;
void ffly_chunk_cleanup();
ffly_lotpp ffly_fetch_lot(ffly_chunkp, mdl_uint_t, mdl_uint_t, mdl_uint_t);
void ffly_chunk_prepare(ffly_chunkp, mdl_uint_t, mdl_uint_t, mdl_uint_t);
ffly_chunkp ffly_alloc_chunk(mdl_uint_t, mdl_uint_t, mdl_uint_t, mdl_u8_t);
void ffly_free_chunk(ffly_chunkp);
# endif /*__ffly__chunk__h*/
