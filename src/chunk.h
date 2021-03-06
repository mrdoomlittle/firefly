# ifndef __ffly__chunk__h
# define __ffly__chunk__h
# include "ffint.h"
# include "lot.h"
# include "types/no_t.h"
# define lotsplice(__al, __lotsize) \
    (__al>>__lotsize)

typedef struct ffly_chunk {
    ff_u8_t lotsize;
	ffly_lotpp top, end;
    ffly_lotppp lots;
    ff_uint_t lotx, loty, lotz;
    ff_uint_t xl, yl, zl;
    ff_uint_t x, y, z;
    ffly_no_t no;

	ff_uint_t size;
    // cleanup
    struct ffly_chunk **bk, *next;
} *ffly_chunkp;

# ifdef __cplusplus
extern "C" {
# endif
ffly_lotp ffly_chunk_lot(ffly_chunkp, ff_uint_t, ff_uint_t, ff_uint_t);
void ffly_chunk_cleanup(void);
void ffly_chunk_update(ffly_chunkp);
ffly_lotp ffly_fetch_lot(ffly_chunkp, ff_uint_t, ff_uint_t, ff_uint_t);
void ffly_chunk_prepare(ffly_chunkp, ff_uint_t, ff_uint_t, ff_uint_t);
ffly_chunkp ffly_chunk_alloc(ff_uint_t, ff_uint_t, ff_uint_t, ff_u8_t);
void ffly_chunk_free(ffly_chunkp);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__chunk__h*/
