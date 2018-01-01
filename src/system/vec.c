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
# define is_set(__v) (__v&0x1)
ffly_off_t ffly_vec_off(ffly_vecp __vec, void *__p) {
	return (mdl_u8_t*)__p-(mdl_u8_t*)__vec->p;
}

void* ffly_vec_at(ffly_vecp __vec, mdl_uint_t __off) {
    return (void*)((mdl_u8_t*)__vec->p+(__off*__vec->blk_size));
}

mdl_uint_t ffly_vec_blk_off(ffly_vecp __vec, void *__p) {
	ffly_off_t off = ffly_vec_off(__vec, __p);
	if (is_flag(__vec, VEC_BLK_CHAIN))
		return (off-sizeof(struct ffly_vec_blkd))/__vec->blk_size;
	return off/__vec->blk_size;
}

void* ffly_vec_begin(ffly_vecp __vec) {
	void *p = __vec->p;
	if (is_flag(__vec, VEC_BLK_CHAIN))
		p = (mdl_u8_t*)p+sizeof(struct ffly_vec_blkd);
	return p;
}

void* ffly_vec_end(ffly_vecp __vec) {
	void *p;
	p = (void*)((mdl_u8_t*)__vec->p+((__vec->off-1)*__vec->blk_size));
	if (is_flag(__vec, VEC_BLK_CHAIN))
		p = (mdl_u8_t*)p+sizeof(struct ffly_vec_blkd);
	return p;
}

ffly_err_t ffly_vec_init(ffly_vecp __vec, ffly_size_t __blk_size) {
	__vec->p = NULL;
	__vec->page_c = 0;

	__vec->blk_size = __blk_size;
	if (is_flag(__vec, VEC_BLK_CHAIN))
		__vec->blk_size+= sizeof(struct ffly_vec_blkd);

	if (is_flag(__vec, VEC_AUTO_RESIZE)) {
		if ((__vec->p = __ffly_mem_alloc((++__vec->page_c)*(VEC_PAGE_SIZE*__vec->blk_size))) == NULL) {
			ffly_fprintf(ffly_err, "vec: failed to alloc memory, errno: %d.\n", errno);
			return FFLY_FAILURE;
		}
	}

	__vec->top = 0;
	__vec->end = 0;
	__vec->off = 0;
	__vec->size = 0;
	if (is_flag(__vec, VEC_BLK_CHAIN)) {
		__vec->uu_blks = (ffly_vecp)__ffly_mem_alloc(sizeof(struct ffly_vec));
		__vec->uu_blks->flags = VEC_AUTO_RESIZE;
		if (ffly_vec_init(__vec->uu_blks, sizeof(ffly_off_t)) != FFLY_SUCCESS) {
			ffly_fprintf(ffly_err, "vec: failed to init uu_blks->\n");
			return FFLY_FAILURE;
		}
	} else
		__vec->uu_blks = NULL;
	return FFLY_SUCCESS;
}

void ffly_vec_itr(ffly_vecp __vec, void **__p, mdl_u8_t __dir, mdl_uint_t __ia) {
	while(__ia != 0) {
		struct ffly_vec_blkd *blk = (struct ffly_vec_blkd*)(((mdl_u8_t*)*__p)-sizeof(struct ffly_vec_blkd));
		if (__dir == VEC_ITR_FD) {
			if (!is_set(blk->next)) {*__p = NULL;return;}
			*__p = (void*)(((mdl_u8_t*)__vec->p+sizeof(struct ffly_vec_blkd))+((blk->next>>1)*__vec->blk_size));
		} else if (__dir == VEC_ITR_BK) {
			if (!is_set(blk->prev)) {*__p = NULL;return;}
			*__p = (void*)(((mdl_u8_t*)__vec->p+sizeof(struct ffly_vec_blkd))+((blk->prev>>1)*__vec->blk_size));
		}
		__ia--;
	}
}

void* ffly_vec_rbegin(ffly_vecp __vec) {
	void *itr = (void*)((mdl_u8_t*)ffly_vec_begin(__vec)-sizeof(struct ffly_vec_blkd));
	while(ffly_is_flag(((struct ffly_vec_blkd*)itr)->flags, VEC_BLK_USED))
		itr = (mdl_u8_t*)itr+__vec->blk_size;
	return (void*)((mdl_u8_t*)itr+sizeof(struct ffly_vec_blkd));
}

void* ffly_vec_rend(ffly_vecp __vec) {
	void *itr = (void*)((mdl_u8_t*)ffly_vec_end(__vec)-sizeof(struct ffly_vec_blkd));
	while(ffly_is_flag(((struct ffly_vec_blkd*)itr)->flags, VEC_BLK_USED))
		itr = (mdl_u8_t*)itr-__vec->blk_size;
	return (void*)((mdl_u8_t*)itr+sizeof(struct ffly_vec_blkd));
}

void* ffly_vec_first(ffly_vecp __vec) {
	return (void*)((mdl_u8_t*)__vec->p+sizeof(struct ffly_vec_blkd)+((__vec->top>>1)*__vec->blk_size));}

void* ffly_vec_last(ffly_vecp __vec) {
	return (void*)((mdl_u8_t*)__vec->p+sizeof(struct ffly_vec_blkd)+((__vec->end>>1)*__vec->blk_size));}

void ffly_vec_dechain(ffly_vecp __vec, struct ffly_vec_blkd *__blk) {
	struct ffly_vec_blkd *prev = (struct ffly_vec_blkd*)((mdl_u8_t*)__vec->p+((__blk->prev>>1)*__vec->blk_size));
	struct ffly_vec_blkd *next = (struct ffly_vec_blkd*)((mdl_u8_t*)__vec->p+((__blk->next>>1)*__vec->blk_size));
	if (is_set(__vec->top)) {
	if ((mdl_u8_t*)__blk-(mdl_u8_t*)__vec->p == (__vec->top>>1)*__vec->blk_size)
		__vec->top = ((ffly_vec_off(__vec, (void*)next)/__vec->blk_size)<<1)|0x1;
	}

	if (is_set(__vec->end)) {
	if ((mdl_u8_t*)__blk-(mdl_u8_t*)__vec->p == (__vec->end>>1)*__vec->blk_size)
		__vec->end = ((ffly_vec_off(__vec, (void*)prev)/__vec->blk_size)<<1)|0x1;
	}

	ffly_printf("%u-%u\n", __blk->prev&0x1, __blk->next&0x1);

	if (is_set(__blk->prev)) {
		if (is_set(__blk->next))
			prev->next = ((ffly_vec_off(__vec, (void*)next)/__vec->blk_size)<<1)|0x1;
		else
			prev->next = 0;
	}

	if (is_set(__blk->next)) {
		if (is_set(__blk->prev))
			next->prev = ((ffly_vec_off(__vec, (void*)prev)/__vec->blk_size)<<1)|0x1;
		else
			next->prev = 0;
	}
}

void ffly_vec_del(ffly_vecp __vec, void *__p) {
	__vec->size--;
	struct ffly_vec_blkd *blk = (struct ffly_vec_blkd*)((mdl_u8_t*)__p-sizeof(struct ffly_vec_blkd));

	ffly_rm_flag(&blk->flags, VEC_BLK_USED);
	ffly_add_flag(&blk->flags, VEC_BLK_FREE, 0);

	ffly_vec_dechain(__vec, blk);

	ffly_off_t *p;
	ffly_vec_push_back(__vec->uu_blks, (void**)&p);
	*p = ((mdl_u8_t*)blk-(mdl_u8_t*)__vec->p)/__vec->blk_size;
}

ffly_err_t ffly_vec_push_back(ffly_vecp __vec, void **__p) {
	__vec->size++;
	if (is_flag(__vec, VEC_BLK_CHAIN) && is_flag(__vec, VEC_UUU_BLKS)) {
		if (ffly_vec_size(__vec->uu_blks) > 0) {
			ffly_off_t off;
			ffly_vec_pop_back(__vec->uu_blks, (void*)&off);
			struct ffly_vec_blkd *blk = (struct ffly_vec_blkd*)((mdl_u8_t*)__vec->p+(off*__vec->blk_size));
			*blk = (struct ffly_vec_blkd) {
				.flags = VEC_BLK_USED,
				.prev = __vec->end,
				.next = 0
			};

			((struct ffly_vec_blkd*)((mdl_u8_t*)__vec->p+((__vec->end>>1)*__vec->blk_size)))->next = (off<<1)|1;
			*__p = (void*)((mdl_u8_t*)blk+sizeof(struct ffly_vec_blkd));
			__vec->end = (off<<1)|1;
			return FFLY_SUCCESS;
		}
	}

	if (is_flag(__vec, VEC_AUTO_RESIZE)) {
		if (__vec->off>>5 > (__vec->off-1)>>5) {
			if ((__vec->p = __ffly_mem_realloc(__vec->p, (++__vec->page_c)*(VEC_PAGE_SIZE*__vec->blk_size))) == NULL) {
				ffly_fprintf(ffly_err, "vec: failed to realloc memory for next page, errno: %d.\n", errno);
				return FFLY_FAILURE;
			}
		}
	}

	*__p = (void*)((mdl_u8_t*)__vec->p+(__vec->off*__vec->blk_size));

	if (is_flag(__vec, VEC_BLK_CHAIN)) {
		struct ffly_vec_blkd *blk = (struct ffly_vec_blkd*)((mdl_u8_t*)*__p);
		*__p = (void*)(((mdl_u8_t*)*__p)+sizeof(struct ffly_vec_blkd));
		blk->prev = blk->next = 0;
		blk->flags = VEC_BLK_USED;

		if (is_set(__vec->end)) {
			blk->prev = __vec->end;

			((struct ffly_vec_blkd*)((mdl_u8_t*)__vec->p+((__vec->end>>1)*__vec->blk_size)))->next = (__vec->off<<1)|1;
		}

		__vec->end = (__vec->off<<1)|1;
		if (!is_set(__vec->top))
			__vec->top = __vec->end;
	}

	__vec->off++;
	return FFLY_SUCCESS;
}
ffly_err_t ffly_vec_pop_back(ffly_vecp __vec, void *__p) {
	__vec->size--;
	if (is_flag(__vec, VEC_AUTO_RESIZE)) {
		// might cause issues - need to test
		if (__vec->off>>5 < (__vec->off+1)>>5 && __vec->page_c > 0) {
			if ((__vec->p = __ffly_mem_realloc(__vec->p, (--__vec->page_c)*(VEC_PAGE_SIZE*__vec->blk_size))) == NULL) {
				ffly_fprintf(ffly_err, "vec: failed to realloc memory for next page, errno: %d.\n", errno);
				return FFLY_FAILURE;
			}
		}
	}

	if (is_flag(__vec, VEC_BLK_CHAIN)) {
		void *last = ffly_vec_last(__vec);
		struct ffly_vec_blkd *blk = (struct ffly_vec_blkd*)((mdl_u8_t*)last-sizeof(struct ffly_vec_blkd));
		ffly_mem_cpy(__p, last, __vec->blk_size);
		if (blk == (void*)((mdl_u8_t*)__vec->p+((__vec->off-1)*__vec->blk_size))) {
			ffly_vec_dechain(__vec, blk);
			__vec->off--;
		} else
			ffly_vec_del(__vec, last);
		ffly_printf("--> %p, %u, %p-%p\n", last, __vec->off, last, (void*)((mdl_u8_t*)__vec->p+((__vec->off-1)*__vec->blk_size)));
	} else {
		// dont change - we cant return the ptr because if a resize takes place the pointer will be invalid
		ffly_mem_cpy(__p, (void*)((mdl_u8_t*)__vec->p+((__vec->off-1)*__vec->blk_size)), __vec->blk_size);
		__vec->off--;
	}
	return FFLY_SUCCESS;
}

ffly_err_t ffly_vec_resize(ffly_vecp __vec, ffly_size_t __size) {
	__vec->size = __size;
	if (!__size) {__ffly_mem_free(__vec->p); __vec->p = NULL; return FFLY_SUCCESS;}
	if (!__vec->p) {__vec->p = __ffly_mem_alloc(__size*__vec->blk_size); return FFLY_SUCCESS;}
	if ((__vec->p = __ffly_mem_realloc(__vec->p, __size*__vec->blk_size)) == NULL) {
		ffly_fprintf(ffly_err, "vec: failed to realloc memory for resize, errno: %d\n", errno);
		return FFLY_FAILURE;}
	ffly_fprintf(ffly_log, "resize.\n");
	return FFLY_SUCCESS;
}

ffly_err_t ffly_vec_de_init(ffly_vecp __vec) {
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
