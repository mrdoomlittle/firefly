# include "chunk.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/io.h"
// is lotsize a square number?
# define workable_lotsize(__size) \
   !(((__size)-(((__size)>>1)*(1<<1)))>0)
# define is_sliceable(__al, __lotsize) \
    !((__al-(lotsplice(__al, __lotsize)*(1<<__lotsize)))>0)

static ffly_chunkp top = NULL;
static ffly_chunkp end = NULL;
ffly_lotpp ffly_fetch_lot(ffly_chunkp __chunk, mdl_uint_t __x, mdl_uint_t __y, mdl_uint_t __z) {
    mdl_uint_t x = lotsplice(__x-__chunk->x, __chunk->lotsize);
    mdl_uint_t y = lotsplice(__y-__chunk->y, __chunk->lotsize);
    mdl_uint_t z = lotsplice(__z-__chunk->z, __chunk->lotsize);
//    ffly_fprintf(ffly_log, "fetching lot at %ux%ux%u or %ux%ux%u, lotsize: %u, chunk: %ux%ux%u\n", x, y, z, __xa, __ya, __za, 1<<__chunk->lotsize, __chunk->xa, __chunk->ya, __chunk->za);
    return __chunk->lots+x+(y*__chunk->lotx)+(z*(__chunk->loty*__chunk->lotx));
}

void ffly_chunk_prepare(ffly_chunkp __chunk, mdl_uint_t __x, mdl_uint_t __y, mdl_uint_t __z) {
    __chunk->x = __x;
    __chunk->y = __y;
    __chunk->z = __z;
}

ffly_chunkp ffly_alloc_chunk(mdl_uint_t __xl, mdl_uint_t __yl, mdl_uint_t __zl, mdl_u8_t __lotsize) {
    if (!workable_lotsize(1<<__lotsize)) {
        ffly_fprintf(ffly_err, "lotsize not workable, %u\n", 1<<__lotsize);
        return NULL;
    }

    if (!is_sliceable(__xl, __lotsize)) {
        ffly_fprintf(ffly_err, "length of x{%u} is unsliceable.\n", __xl);
        return NULL;
    }

    if (!is_sliceable(__yl, __lotsize)) {
        ffly_fprintf(ffly_err, "length of y{%u} is unsliceable.\n", __yl);
        return NULL;
    }

    if (!is_sliceable(__zl, __lotsize)) {
        ffly_fprintf(ffly_err, "length of z{%u} is unsliceable.\n", __zl);
        return NULL;
    }    

    ffly_chunkp chunk = (ffly_chunkp)__ffly_mem_alloc(sizeof(struct ffly_chunk));
    chunk->next = NULL;
    chunk->prev = NULL;
    if (!top) {
        top = chunk;
        chunk->no = 0;
    }

    if (end != NULL) {
        end->next = chunk;
        chunk->prev = end;
        end = chunk;
        chunk->no = chunk->prev->no+1;
    }

    end = chunk;

    chunk->lotx = lotsplice(__xl, __lotsize);
    chunk->loty = lotsplice(__yl, __lotsize);
    chunk->lotz = lotsplice(__zl, __lotsize);

    chunk->lots = (ffly_lotpp)__ffly_mem_alloc((chunk->lotx*chunk->loty*chunk->lotz)*sizeof(ffly_lotp));
    mdl_uint_t x, y, z = 0;
    ffly_fprintf(ffly_log, "new chunk, %u-%u-%u\n", __xl, __yl, __zl);
    while(z != chunk->lotz) {
        y = 0;
        while(y != chunk->loty) {
            x = 0;
            while(x != chunk->lotx) {
                ffly_fprintf(ffly_log, "setting lot null, %ux%ux%u\n", x, y, z);
                *(chunk->lots+x+(y*chunk->lotx)+(z*(chunk->loty*chunk->lotx))) = NULL;
                x++;
            }
            y++;
        }
        z++;
    }
    chunk->xl = __xl;
    chunk->yl = __yl;
    chunk->zl = __zl;
    chunk->lotsize = __lotsize;
    return chunk; 
}

void ffly_free_chunk(ffly_chunkp __chunk) {
    __ffly_mem_free(__chunk->lots);
}

void ffly_chunk_cleanup() {
    ffly_fprintf(ffly_log, "cleaning up chunks.\n");
    ffly_chunkp chunk = top, prev = NULL;
    while(chunk != NULL) {
        if (prev != NULL)
            __ffly_mem_free(prev);
        ffly_free_chunk(chunk);
        prev = chunk;
        chunk = chunk->next;
    }

    if (end != NULL)
         __ffly_mem_free(end);
}
