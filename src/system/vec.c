# include "vec.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# include "io.h"
# include "../data/mem_ncpy.h"
# include "errno.h"
# include <errno.h>
# include <eint_t.h>
# define VEC_BLK_USED 1
# define VEC_BLK_FREE 0
ffly_bool_t static is_flag(struct ffly_vec *__vec, mdl_u8_t __flag) {
	return (__vec->flags & __flag) == __flag?1:0;}

ffly_err_t ffly_vec_init(struct ffly_vec *__vec, mdl_uint_t __blk_size) {
	__vec->p = NULL;
	__vec->page_c = 0;

	__vec->blk_size = __blk_size;
	if (is_flag(__vec, VEC_BLK_CHAIN))
		__vec->blk_size += sizeof(struct ffly_vec_chain);

	if (is_flag(__vec, VEC_AUTO_RESIZE)) {
		if ((__vec->p = __ffly_mem_alloc((++__vec->page_c)*(VEC_PAGE_SIZE*__vec->blk_size))) == NULL) {
			ffly_printf(stderr, "vec: failed to alloc memory, errno: %d.\n", errno);
			return FFLY_FAILURE;
		}
	}

	__vec->last_blk = 0;
	__vec->off = 0;
	__vec->size = 0;
	return FFLY_SUCCESS;
}

void ffly_vec_itr(struct ffly_vec *__vec, void **__p, mdl_u8_t __dir) {
	struct ffly_vec_chain *blk_desc = (struct ffly_vec_chain*)(((mdl_u8_t*)*__p)-sizeof(struct ffly_vec_chain));
	if (__dir == VEC_ITR_UP) {
		if (!(blk_desc->above & 0x1)) {*__p = NULL;return;}
		*__p = (void*)(((mdl_u8_t*)__vec->p+sizeof(struct ffly_vec_chain))+((blk_desc->above>>1)*__vec->blk_size));
	} else if (__dir == VEC_ITR_DOWN) {
		if (!(blk_desc->below & 0x1)) {*__p = NULL;return;}
		printf("jumping to: %u\n", blk_desc->below>>1);
		*__p = (void*)(((mdl_u8_t*)__vec->p+sizeof(struct ffly_vec_chain))+((blk_desc->below>>1)*__vec->blk_size));
	}
}

void* ffly_vec_rbegin(struct ffly_vec *__vec) {
	void *itr = (void*)((mdl_u8_t*)ffly_vec_begin(__vec)-sizeof(struct ffly_vec_chain));
	while(((struct ffly_vec_chain*)itr)->state != VEC_BLK_USED) {
		itr = (mdl_u8_t*)itr+__vec->blk_size;
	}
	return (void*)((mdl_u8_t*)itr+sizeof(struct ffly_vec_chain));
}

void* ffly_vec_rend(struct ffly_vec *__vec) {
	void *itr = (void*)((mdl_u8_t*)ffly_vec_end(__vec)-sizeof(struct ffly_vec_chain));
	while(((struct ffly_vec_chain*)itr)->state != VEC_BLK_USED) {
		itr = (mdl_u8_t*)itr-__vec->blk_size;
	}
	return (void*)((mdl_u8_t*)itr+sizeof(struct ffly_vec_chain));
}

void ffly_vec_del(struct ffly_vec *__vec, void *__p) {
	struct ffly_vec_chain *blk_desc = (struct ffly_vec_chain*)((mdl_u8_t*)__p-sizeof(struct ffly_vec_chain));

	struct ffly_vec_chain *above = (struct ffly_vec_chain*)((mdl_u8_t*)__vec->p+((blk_desc->above>>1)*__vec->blk_size));
	struct ffly_vec_chain *below = (struct ffly_vec_chain*)((mdl_u8_t*)__vec->p+((blk_desc->below>>1)*__vec->blk_size));

	mdl_u8_t flg = 0;
	while(flg != 3) {
		if (blk_desc->above & 0x1) {
			if (above->above & 0x1 && above->state == VEC_BLK_FREE) {
				above = (struct ffly_vec_chain*)((mdl_u8_t*)__vec->p+((above->above>>1)*__vec->blk_size));
			} else flg |= 1;
		} else flg |= 1;

		if (blk_desc->below & 0x1) {
			if (below->below & 0x1 && below->state == VEC_BLK_FREE) {
				below = (struct ffly_vec_chain*)((mdl_u8_t*)__vec->p+((below->below>>1)*__vec->blk_size));
			} else flg |= 2;
		} else flg |= 2;
	}

	blk_desc->state = VEC_BLK_FREE;

	if (!(blk_desc->above & 0x1) || (!(blk_desc->above & 0x1) && !(blk_desc->below & 0x1))) __vec->last_blk = 0;
	if (!(blk_desc->below & 0x1) && blk_desc->above & 0x1) {
		__vec->last_blk = ((((mdl_u8_t*)above-(mdl_u8_t*)__vec->p)/__vec->blk_size) << 1) | 1;
		above->below = 0;
	}

	if (blk_desc->above & 0x1 && blk_desc->below & 0x1) {
		above->below = ((((mdl_u8_t*)below-(mdl_u8_t*)__vec->p)/__vec->blk_size) << 1) | 1;
		below->above = ((((mdl_u8_t*)above-(mdl_u8_t*)__vec->p)/__vec->blk_size) << 1) | 1;
	}
}

ffly_err_t ffly_vec_push_back(struct ffly_vec *__vec, void **__p) {
	if (is_flag(__vec, VEC_AUTO_RESIZE)) {
		for (;;) {
		if (__vec->off >> 5 > (__vec->off-1 >> 5)) {
			if ((__vec->p = __ffly_mem_realloc(__vec->p, (++__vec->page_c)*(VEC_PAGE_SIZE*__vec->blk_size))) == NULL) {
				ffly_printf(stderr, "vec: failed to realloc memory for next page, errno: %d.\n", errno);
				return FFLY_FAILURE;
			}
		} else break;
		}
	}

	*__p = (void*)((mdl_u8_t*)__vec->p+(__vec->off*__vec->blk_size));

	if (is_flag(__vec, VEC_BLK_CHAIN)) {
		*__p = (void*)(((mdl_u8_t*)*__p)+sizeof(struct ffly_vec_chain));
		struct ffly_vec_chain *blk_desc = (struct ffly_vec_chain*)(((mdl_u8_t*)*__p)-sizeof(struct ffly_vec_chain));
		blk_desc->above = blk_desc->below = 0;
		blk_desc->state = VEC_BLK_USED;

		if (__vec->last_blk & 0x1) {
			blk_desc->above = __vec->last_blk;

			((struct ffly_vec_chain*)((mdl_u8_t*)__vec->p+((__vec->last_blk>>1)*__vec->blk_size)))->below = (__vec->off << 1) | 1;
		}

		__vec->last_blk = (__vec->off << 1) | 1;
	}

	__vec->off++;
	return FFLY_SUCCESS;
}

/*
	if theres a error with pop move ffly_byte_ncpy to the top
*/
ffly_err_t ffly_vec_pop_back(struct ffly_vec *__vec, void *__p) {
	if (is_flag(__vec, VEC_AUTO_RESIZE)) {
		for (;;) {
		if (__vec->off >> 5 < (__vec->off+1 >> 5) && __vec->page_c > 1) {
			if ((__vec->p = __ffly_mem_realloc(__vec->p, (--__vec->page_c)*(VEC_PAGE_SIZE*__vec->blk_size))) == NULL) {
				ffly_printf(stderr, "vec: failed to realloc memory for next page, errno: %d.\n", errno);
				return FFLY_FAILURE;
			}
		} else break;
		}
	}

	if (is_flag(__vec, VEC_BLK_CHAIN)) {
		__p = (void*)((mdl_u8_t*)__p+sizeof(struct ffly_vec_chain));
	}

	// dont change - we cant return the ptr because if a resize takes place the pointer will be invalid
	ffly_mem_ncpy(__p, (void*)((mdl_u8_t*)__vec->p+((__vec->off-1)*__vec->blk_size)), __vec->blk_size);
	__vec->off--;
	return FFLY_SUCCESS;
}

ffly_err_t ffly_vec_resize(struct ffly_vec *__vec, mdl_uint_t __size) {
	__vec->size = __size;
	if (!__size) {__ffly_mem_free(__vec->p); __vec->p = NULL; return FFLY_SUCCESS;}
	if (!__vec->p) {__vec->p = __ffly_mem_alloc(__size*__vec->blk_size); return FFLY_SUCCESS;}
	if ((__vec->p = __ffly_mem_realloc(__vec->p, __size*__vec->blk_size)) == NULL) {
		ffly_printf(stderr, "vec: failed to realloc memory for resize, errno: %d\n", errno);
		return FFLY_FAILURE;}
	ffly_printf(stdout, "resize.\n");
	return FFLY_SUCCESS;
}

ffly_err_t ffly_vec_de_init(struct ffly_vec *__vec) {
	if (__vec->p != NULL) {
		__ffly_mem_free(__vec->p);
		__vec->p = NULL;
	}
	return FFLY_SUCCESS;
}
