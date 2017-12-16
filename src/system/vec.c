# include "vec.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# include "io.h"
# include "../data/mem_cpy.h"
# include "errno.h"
# include <errno.h>
# include <mdlint.h>
# define VEC_BLK_USED 0x1
# define VEC_BLK_FREE 0x2
# define is_flag(__vec, __flag) ffly_is_flag(__vec->flags, __flag)

void* ffly_vec_begin(struct ffly_vec *__vec) {
	void *p = __vec->p;
	if (is_flag(__vec, VEC_BLK_CHAIN))
		p = (mdl_u8_t*)p+sizeof(struct ffly_vec_chain);
	return p;
}

void* ffly_vec_end(struct ffly_vec *__vec) {
	void *p;
	p = (void*)((mdl_u8_t*)__vec->p+((__vec->off-1)*__vec->blk_size));
	if (is_flag(__vec, VEC_BLK_CHAIN))
		p = (mdl_u8_t*)p+sizeof(struct ffly_vec_chain);
	return p;
}

ffly_err_t ffly_vec_init(struct ffly_vec *__vec, ffly_size_t __blk_size) {
	__vec->p = NULL;
	__vec->page_c = 0;

	__vec->blk_size = __blk_size;
	if (is_flag(__vec, VEC_BLK_CHAIN))
		__vec->blk_size+= sizeof(struct ffly_vec_chain);

	if (is_flag(__vec, VEC_AUTO_RESIZE)) {
		if ((__vec->p = __ffly_mem_alloc((++__vec->page_c)*(VEC_PAGE_SIZE*__vec->blk_size))) == NULL) {
			ffly_fprintf(ffly_err, "vec: failed to alloc memory, errno: %d.\n", errno);
			return FFLY_FAILURE;
		}
	}

	__vec->first_blk = 0;
	__vec->last_blk = 0;
	__vec->off = 0;
	__vec->size = 0;
	if (is_flag(__vec, VEC_BLK_CHAIN)) {
		__vec->uu_blks = (struct ffly_vec*)__ffly_mem_alloc(sizeof(struct ffly_vec));
		__vec->uu_blks->flags = VEC_AUTO_RESIZE;
		if (ffly_vec_init(__vec->uu_blks, sizeof(mdl_uint_t*)) != FFLY_SUCCESS) {
			ffly_fprintf(ffly_err, "vec: failed to init uu_blks->\n");
			return FFLY_FAILURE;
		}
	} else
		__vec->uu_blks = NULL;
	return FFLY_SUCCESS;
}

void ffly_vec_itr(struct ffly_vec *__vec, void **__p, mdl_u8_t __dir, mdl_uint_t __ia) {
	while(__ia != 0) {
		struct ffly_vec_chain *blk_chain = (struct ffly_vec_chain*)(((mdl_u8_t*)*__p)-sizeof(struct ffly_vec_chain));
		if (__dir == VEC_ITR_FD) {
			if (!(blk_chain->below&0x1)) {*__p = NULL;return;}
			*__p = (void*)(((mdl_u8_t*)__vec->p+sizeof(struct ffly_vec_chain))+((blk_chain->below>>1)*__vec->blk_size));
		} else if (__dir == VEC_ITR_BK) {
			if (!(blk_chain->above&0x1)) {*__p = NULL;return;}
			*__p = (void*)(((mdl_u8_t*)__vec->p+sizeof(struct ffly_vec_chain))+((blk_chain->above>>1)*__vec->blk_size));
		}
		__ia--;
	}
}

void* ffly_vec_rbegin(struct ffly_vec *__vec) {
	void *itr = (void*)((mdl_u8_t*)ffly_vec_begin(__vec)-sizeof(struct ffly_vec_chain));
	while(ffly_is_flag(((struct ffly_vec_chain*)itr)->flags, VEC_BLK_USED))
		itr = (mdl_u8_t*)itr+__vec->blk_size;
	return (void*)((mdl_u8_t*)itr+sizeof(struct ffly_vec_chain));
}

void* ffly_vec_rend(struct ffly_vec *__vec) {
	void *itr = (void*)((mdl_u8_t*)ffly_vec_end(__vec)-sizeof(struct ffly_vec_chain));
	while(ffly_is_flag(((struct ffly_vec_chain*)itr)->flags, VEC_BLK_USED))
		itr = (mdl_u8_t*)itr-__vec->blk_size;
	return (void*)((mdl_u8_t*)itr+sizeof(struct ffly_vec_chain));
}

void* ffly_vec_first(struct ffly_vec *__vec) {
	return (void*)((mdl_u8_t*)__vec->p+sizeof(struct ffly_vec_chain)+((__vec->first_blk>>1)*__vec->blk_size));}

void* ffly_vec_last(struct ffly_vec *__vec) {
	return (void*)((mdl_u8_t*)__vec->p+sizeof(struct ffly_vec_chain)+((__vec->last_blk>>1)*__vec->blk_size));}

void ffly_vec_del(struct ffly_vec *__vec, void *__p) {
	__vec->size--;
	struct ffly_vec_chain *blk_chain = (struct ffly_vec_chain*)((mdl_u8_t*)__p-sizeof(struct ffly_vec_chain));

	struct ffly_vec_chain *above = (struct ffly_vec_chain*)((mdl_u8_t*)__vec->p+((blk_chain->above>>1)*__vec->blk_size));
	struct ffly_vec_chain *below = (struct ffly_vec_chain*)((mdl_u8_t*)__vec->p+((blk_chain->below>>1)*__vec->blk_size));

	if (blk_chain->above&0x1) {
		while(ffly_is_flag(above->flags, VEC_BLK_FREE)) {
			above = (struct ffly_vec_chain*)((mdl_u8_t*)__vec->p+((above->above>>1)*__vec->blk_size));
			if (!(above->above&0x1)) break;
		}
	}

	if (blk_chain->below&0x1) {
		while(ffly_is_flag(below->flags, VEC_BLK_FREE)) {
			below = (struct ffly_vec_chain*)((mdl_u8_t*)__vec->p+((below->below>>1)*__vec->blk_size));
			if (!(below->below&0x1)) break;
		}
	}

	if ((mdl_u8_t*)blk_chain-(mdl_u8_t*)__vec->p == (__vec->first_blk>>1)*__vec->blk_size)
		__vec->first_blk = ((((mdl_u8_t*)below-(mdl_u8_t*)__vec->p)/__vec->blk_size)<<1)|1;

	ffly_add_flag(&blk_chain->flags, VEC_BLK_FREE, 0);

	if (!(blk_chain->above&0x1) || (!(blk_chain->above&0x1) && !(blk_chain->below&0x1)))
		__vec->last_blk = 0;
	if (!(blk_chain->below&0x1) && blk_chain->above&0x1) {
		__vec->last_blk = ((((mdl_u8_t*)above-(mdl_u8_t*)__vec->p)/__vec->blk_size)<<1)|1;
		above->below = 0;
	}

	if (blk_chain->above&0x1 && blk_chain->below&0x1) {
		above->below = ((((mdl_u8_t*)below-(mdl_u8_t*)__vec->p)/__vec->blk_size)<<1)|1;
		below->above = ((((mdl_u8_t*)above-(mdl_u8_t*)__vec->p)/__vec->blk_size)<<1)|1;
	}

	mdl_uint_t *p;
	ffly_vec_push_back(__vec->uu_blks, (void**)&p);
	*p = ((mdl_u8_t*)blk_chain-(mdl_u8_t*)__vec->p)/__vec->blk_size;
}

ffly_err_t ffly_vec_push_back(struct ffly_vec *__vec, void **__p) {
	__vec->size++;
	if (is_flag(__vec, VEC_BLK_CHAIN) && is_flag(__vec, VEC_UUU_BLKS)) {
		if (ffly_vec_size(__vec->uu_blks) > 0) {
			ffly_off_t off;
			ffly_vec_pop_back(__vec->uu_blks, (void*)&off);
			struct ffly_vec_chain *blk_chain = (struct ffly_vec_chain*)((mdl_u8_t*)__vec->p+(off*__vec->blk_size));
			*blk_chain = (struct ffly_vec_chain) {
				.flags = VEC_BLK_USED,
				.above = __vec->last_blk,
				.below = 0
			};

			((struct ffly_vec_chain*)((mdl_u8_t*)__vec->p+((__vec->last_blk>>1)*__vec->blk_size)))->below = (off<<1)|1;
			*__p = (void*)((mdl_u8_t*)blk_chain+sizeof(struct ffly_vec_chain));
			__vec->last_blk = (off<<1)|1;
			return FFLY_SUCCESS;
		}
	}

	if (is_flag(__vec, VEC_AUTO_RESIZE)) {
		for (;;) {
		if (__vec->off>>5 > (__vec->off-1)>>5) {
			if ((__vec->p = __ffly_mem_realloc(__vec->p, (++__vec->page_c)*(VEC_PAGE_SIZE*__vec->blk_size))) == NULL) {
				ffly_fprintf(ffly_err, "vec: failed to realloc memory for next page, errno: %d.\n", errno);
				return FFLY_FAILURE;
			}
		} else break;
		}
	}

	*__p = (void*)((mdl_u8_t*)__vec->p+(__vec->off*__vec->blk_size));

	if (is_flag(__vec, VEC_BLK_CHAIN)) {
		struct ffly_vec_chain *blk_chain = (struct ffly_vec_chain*)((mdl_u8_t*)*__p);
		*__p = (void*)(((mdl_u8_t*)*__p)+sizeof(struct ffly_vec_chain));
		blk_chain->above = blk_chain->below = 0;
		blk_chain->flags = VEC_BLK_USED;

		if (__vec->last_blk&0x1) {
			blk_chain->above = __vec->last_blk;

			((struct ffly_vec_chain*)((mdl_u8_t*)__vec->p+((__vec->last_blk>>1)*__vec->blk_size)))->below = (__vec->off<<1)|1;
		}

		__vec->last_blk = (__vec->off<<1)|1;
		if (!(__vec->first_blk&0x1))
			__vec->first_blk = __vec->last_blk;
	}

	__vec->off++;
	return FFLY_SUCCESS;
}

/*
	if theres a error with pop move ffly_byte_ncpy to the top
*/
ffly_err_t ffly_vec_pop_back(struct ffly_vec *__vec, void *__p) {
	__vec->size--;
	if (is_flag(__vec, VEC_AUTO_RESIZE)) {
		for (;;) {
		if (__vec->off>>5 < (__vec->off+1)>>5 && __vec->page_c > 1) {
			if ((__vec->p = __ffly_mem_realloc(__vec->p, (--__vec->page_c)*(VEC_PAGE_SIZE*__vec->blk_size))) == NULL) {
				ffly_fprintf(ffly_err, "vec: failed to realloc memory for next page, errno: %d.\n", errno);
				return FFLY_FAILURE;
			}
		} else break;
		}
	}

	if (is_flag(__vec, VEC_BLK_CHAIN))
		__p = (void*)((mdl_u8_t*)__p+sizeof(struct ffly_vec_chain));

	// dont change - we cant return the ptr because if a resize takes place the pointer will be invalid
	ffly_mem_cpy(__p, (void*)((mdl_u8_t*)__vec->p+((__vec->off-1)*__vec->blk_size)), __vec->blk_size);
	__vec->off--;
	return FFLY_SUCCESS;
}

ffly_err_t ffly_vec_resize(struct ffly_vec *__vec, ffly_size_t __size) {
	__vec->size = __size;
	if (!__size) {__ffly_mem_free(__vec->p); __vec->p = NULL; return FFLY_SUCCESS;}
	if (!__vec->p) {__vec->p = __ffly_mem_alloc(__size*__vec->blk_size); return FFLY_SUCCESS;}
	if ((__vec->p = __ffly_mem_realloc(__vec->p, __size*__vec->blk_size)) == NULL) {
		ffly_fprintf(ffly_err, "vec: failed to realloc memory for resize, errno: %d\n", errno);
		return FFLY_FAILURE;}
	ffly_fprintf(ffly_log, "resize.\n");
	return FFLY_SUCCESS;
}

ffly_err_t ffly_vec_de_init(struct ffly_vec *__vec) {
	if (__vec->p != NULL) {
		__ffly_mem_free(__vec->p);
		__vec->p = NULL;
	}

	if (__vec->uu_blks != NULL) {
		ffly_vec_de_init(__vec->uu_blks);
		__vec->uu_blks = NULL;
	}
	return FFLY_SUCCESS;
}
