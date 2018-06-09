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
ffly_lotp ffly_chunk_lot(ffly_chunkp __chunk, ff_uint_t __x, ff_uint_t __y, ff_uint_t __z) {
	ff_uint_t x = lotsplice(__x-__chunk->x, __chunk->lotsize);
	ff_uint_t y = lotsplice(__y-__chunk->y, __chunk->lotsize);
	ff_uint_t z = lotsplice(__z-__chunk->z, __chunk->lotsize);
	ffly_lotppp p = __chunk->lots+x+(y*__chunk->lotx)+(z*(__chunk->loty*__chunk->lotx));
	if (p>=__chunk->lots+__chunk->size) {
		ffly_printf("chunk, out of bounds.\n");
		return NULL;
	}

	ff_u8_t lotsize = __chunk->lotsize;

	if (__chunk->end>=__chunk->top+__chunk->size) {
		ffly_printf("chunk no space.\n");
		return NULL;
	}

	if (*p != NULL) {
		ffly_printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@chunk, unclean memory.\n");
		// error
		return NULL;
	}

	ffly_lotp lot;
	lot = (*(*p = __chunk->end++) = ffly_lot_alloc(1<<lotsize, 1<<lotsize, 1<<lotsize));
	ffly_lot_prepare(lot, (__x>>lotsize)*(1<<lotsize), (__y>>lotsize)*(1<<lotsize), (__z>>lotsize)*(1<<lotsize));
	lot->p = p;
	ffly_printf("created new lot : %p, {%u:%u:%u}, {%u:%u:%u} : %u\n", lot, __x, __y, __z, lot->x, lot->y, lot->z, lotsize);
	return lot;
}

ffly_lotp ffly_fetch_lot(ffly_chunkp __chunk, ff_uint_t __x, ff_uint_t __y, ff_uint_t __z) {
	ff_uint_t x = lotsplice(__x-__chunk->x, __chunk->lotsize);
	ff_uint_t y = lotsplice(__y-__chunk->y, __chunk->lotsize);
	ff_uint_t z = lotsplice(__z-__chunk->z, __chunk->lotsize);
//	  ffly_fprintf(ffly_log, "fetching lot at %ux%ux%u or %ux%ux%u, lotsize: %u, chunk: %ux%ux%u\n", x, y, z, __xa, __ya, __za, 1<<__chunk->lotsize, __chunk->xa, __chunk->ya, __chunk->za);

	ffly_lotppp p = __chunk->lots+x+(y*__chunk->lotx)+(z*(__chunk->loty*__chunk->lotx));
	if (!*p)
		return NULL;
	ffly_lotp lot = **p;
	if (lot->p != p) {
		ffly_fprintf(ffly_err, "chunk, lot location differ.\n");
		return NULL;
	}
	return lot;
}

void ffly_chunk_update(ffly_chunkp __chunk) {
	ffly_lotpp cur = __chunk->top;
	ffly_lotp lot;
	//ffly_printf("chunk usage: %u:%u\n", __chunk->lotx*__chunk->loty*__chunk->lotz, __chunk->end-__chunk->top);
	while(cur < __chunk->end) {
		lot = *cur;
		ffly_printf("lotsize: %u\n", lot->end-lot->top);
		if (!lot) {
			ffly_printf("chunk update error.\n");
		} else {
			if (lot->top == lot->end) {
				ffly_printf("removing lot, not in use/empty, %p\n", lot);
				if (cur == __chunk->end-1)
					__chunk->end--;
				else {
					ffly_lotp t = *(--__chunk->end);
					*(*t->p = *lot->p) = t;
				}
				*lot->p = NULL;
				ffly_lot_free(lot);
			}
		}
		cur++;
	}
}

void ffly_chunk_prepare(ffly_chunkp __chunk, ff_uint_t __x, ff_uint_t __y, ff_uint_t __z) {
	__chunk->x = __x;
	__chunk->y = __y;
	__chunk->z = __z;
}

ffly_chunkp ffly_chunk_alloc(ff_uint_t __xl, ff_uint_t __yl, ff_uint_t __zl, ff_u8_t __lotsize) {
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
	if (top != NULL)
		top->bk = &chunk->next;
	chunk->next = top;
	chunk->bk = &top;
	top = chunk;


	chunk->lotx = lotsplice(__xl, __lotsize);
	chunk->loty = lotsplice(__yl, __lotsize);
	chunk->lotz = lotsplice(__zl, __lotsize);

	ff_uint_t size = chunk->lotx*chunk->loty*chunk->lotz;
	chunk->size = size;

	chunk->top = (ffly_lotpp)__ffly_mem_alloc(size*sizeof(ffly_lotp));
	chunk->end = chunk->top;
	chunk->lots = (ffly_lotppp)__ffly_mem_alloc(size*sizeof(ffly_lotpp));
	ff_uint_t x, y, z = 0;
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

void ffly_chunk_free(ffly_chunkp __chunk) {
	*__chunk->bk = __chunk->next;
	if (__chunk->next != NULL)
		__chunk->next->bk = __chunk->bk;

	__ffly_mem_free(__chunk->top);
	__ffly_mem_free(__chunk->lots);
	__ffly_mem_free(__chunk);
}

void ffly_chunk_cleanup(void) {
	ffly_fprintf(ffly_log, "cleaning up chunks.\n");
	ffly_chunkp cur = top, bk;
	while(cur != NULL) {
		ffly_fprintf(ffly_log, "chunk %u cleanup\n", cur->no);
		bk = cur;
		cur = cur->next;
		ffly_chunk_free(bk);
	}
}
