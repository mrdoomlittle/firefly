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
ffly_lotpp ffly_fetch_lot(ffly_chunkp __chunk, mdl_uint_t __xa, mdl_uint_t __ya, mdl_uint_t __za) {
    mdl_uint_t x = lotsplice(__xa-__chunk->xa, __chunk->lotsize);
    mdl_uint_t y = lotsplice(__ya-__chunk->ya, __chunk->lotsize);
    mdl_uint_t z = lotsplice(__za-__chunk->za, __chunk->lotsize);
    ffly_fprintf(ffly_log, "fetching lot at %ux%ux%u or %ux%ux%u, lotsize: %u, chunk: %ux%ux%u\n", x, y, z, __xa, __ya, __za, 1<<__chunk->lotsize, __chunk->xa, __chunk->ya, __chunk->za);
    return __chunk->lots+x+(y*__chunk->lotx)+(z*(__chunk->loty*__chunk->lotx));
}

void ffly_chunk_prepare(ffly_chunkp __chunk, mdl_uint_t __xa, mdl_uint_t __ya, mdl_uint_t __za) {
    __chunk->xa = __xa;
    __chunk->ya = __ya;
    __chunk->za = __za;
}

ffly_chunkp ffly_alloc_chunk(mdl_uint_t __xal, mdl_uint_t __yal, mdl_uint_t __zal, mdl_u8_t __lotsize) {
    if (!workable_lotsize(1<<__lotsize)) {
        ffly_fprintf(ffly_err, "lotsize not workable, %u\n", 1<<__lotsize);
        return NULL;
    }

    if (!is_sliceable(__xal, __lotsize)) {
        ffly_fprintf(ffly_err, "length of x{%u} is unsliceable.\n", __xal);
        return NULL;
    }

    if (!is_sliceable(__yal, __lotsize)) {
        ffly_fprintf(ffly_err, "length of y{%u} is unsliceable.\n", __yal);
        return NULL;
    }

    if (!is_sliceable(__zal, __lotsize)) {
        ffly_fprintf(ffly_err, "length of z{%u} is unsliceable.\n", __zal);
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

    chunk->lotx = lotsplice(__xal, __lotsize);
    chunk->loty = lotsplice(__yal, __lotsize);
    chunk->lotz = lotsplice(__zal, __lotsize);

    chunk->lots = (ffly_lotpp)__ffly_mem_alloc((chunk->lotx*chunk->loty*chunk->lotz)*sizeof(ffly_lotp));
    mdl_uint_t xa, ya, za = 0;
    ffly_fprintf(ffly_log, "new chunk, %u-%u-%u\n", __xal, __yal, __zal);
    while(za != chunk->lotz) {
        ya = 0;
        while(ya != chunk->loty) {
            xa = 0;
            while(xa != chunk->lotx) {
                ffly_fprintf(ffly_log, "setting lot null, %ux%ux%u\n", xa, ya, za);
                *(chunk->lots+xa+(ya*chunk->lotx)+(za*(chunk->loty*chunk->lotx))) = NULL;
                xa++;
            }
            ya++;
        }
        za++;
    }
    chunk->xal = __xal;
    chunk->yal = __yal;
    chunk->zal = __zal;
    chunk->lotsize = __lotsize;
    return chunk; 
}

void ffly_free_chunk(ffly_chunkp __chunk) {
    __ffly_mem_free(__chunk->lots);
}

void ffly_chunk_cleanup() {
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
