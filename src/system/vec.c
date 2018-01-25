# include "vec.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# include "io.h"
# include "../data/mem_cpy.h"
# include "errno.h"
# include <errno.h>
# include <mdlint.h>
# define VEC_NULL ((ffly_off_t)~0)
# define VEC_BLK_USED 0x1
# define VEC_BLK_FREE 0x2
# define is_flag(__vec, __flag) ffly_is_flag(__vec->flags, __flag)
# define not_null(__v)(__v!=VEC_NULL)

# define get_at(__vec, __off) \
    ((void*)(((mdl_u8_t*)*((void**)__vec->p+(__off>>VEC_PAGE_SHIFT)))+(((__off)-(((__off)>>VEC_PAGE_SHIFT)*VEC_PAGE_SIZE))*__vec->blk_size)))
ffly_vecp static top = NULL;
ffly_vecp ffly_vec_list() {
    return top;
}

void ffly_vec_attach(ffly_vecp __vec) {
    __vec->prev = NULL;
    __vec->next = top;
    if (!top)
        top = __vec;
    else {
        top->prev = __vec;
        top = __vec; 
    }
}

void ffly_vec_detach(ffly_vecp __vec) {
    if (__vec == top) {
        top = __vec->next;
        if (top != NULL)
            top->prev = NULL;
    } else {
        if (__vec->prev != NULL)
            __vec->prev->next = __vec->next;
        if (__vec->next != NULL)
            __vec->next->prev = __vec->prev;
    }
}

ffly_vecp ffly_vec(ffly_size_t __blk_size, ffly_flag_t __flags, ffly_err_t *__err) { 
    ffly_vecp p = (ffly_vecp)__ffly_mem_alloc(sizeof(struct ffly_vec));
    p->flags = __flags;
    *__err = ffly_vec_init(p, __blk_size);
    ffly_vec_attach(p);
    return p;
}

ffly_err_t ffly_vec_destroy(ffly_vecp __vec) {
    ffly_vec_de_init(__vec);
    ffly_vec_free(__vec);
}

ffly_err_t ffly_vec_free(ffly_vecp __vec) {
    ffly_vec_detach(__vec);
    __ffly_mem_free(__vec);
}

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
	void *p = is_flag(__vec, VEC_NONCONTINUOUS)?(*(void**)__vec->p):__vec->p;
	if (is_flag(__vec, VEC_BLK_CHAIN))
		p = (mdl_u8_t*)p+sizeof(struct ffly_vec_blkd);
	return p;
}

void* ffly_vec_end(ffly_vecp __vec) {
	void *p;
    if (is_flag(__vec, VEC_NONCONTINUOUS)) {
        mdl_uint_t off = __vec->off-1;
        mdl_uint_t page = off>>VEC_PAGE_SHIFT;
        p = ((mdl_u8_t*)*((void**)__vec->p+page))+((off-(page*VEC_PAGE_SIZE))*__vec->blk_size);   
    } else
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
        if (is_flag(__vec, VEC_NONCONTINUOUS)) {
            if ((__vec->p = __ffly_mem_alloc(sizeof(void*))) == NULL) {
                return FFLY_FAILURE;
            }

            *(void**)__vec->p = __ffly_mem_alloc(VEC_PAGE_SIZE*__vec->blk_size);
        } else {
		    if ((__vec->p = __ffly_mem_alloc(VEC_PAGE_SIZE*__vec->blk_size)) == NULL) {
			    ffly_fprintf(ffly_err, "vec: failed to alloc memory, errno: %d.\n", errno);
			    return FFLY_FAILURE;
		    }
        }
        __vec->page_c++;
	}

	__vec->top = VEC_NULL;
	__vec->end = VEC_NULL;
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
			if (!not_null(blk->next)) {*__p = NULL;return;}
            if (is_flag(__vec, VEC_NONCONTINUOUS))
                *__p = (void*)((mdl_u8_t*)get_at(__vec, blk->next)+sizeof(struct ffly_vec_blkd));
            else
			    *__p = (void*)(((mdl_u8_t*)__vec->p+sizeof(struct ffly_vec_blkd))+(blk->next*__vec->blk_size));
		} else if (__dir == VEC_ITR_BK) {
			if (!not_null(blk->prev)) {*__p = NULL;return;}
            if (is_flag(__vec, VEC_NONCONTINUOUS))
                *__p = (void*)((mdl_u8_t*)get_at(__vec, blk->prev)+sizeof(struct ffly_vec_blkd));
            else
			    *__p = (void*)(((mdl_u8_t*)__vec->p+sizeof(struct ffly_vec_blkd))+(blk->prev*__vec->blk_size));
		}
		__ia--;
	}
}

void* ffly_vec_rbegin(ffly_vecp __vec) {
	struct ffly_vec_blkd *blk = (struct ffly_vec_blkd*)((mdl_u8_t*)ffly_vec_begin(__vec)-sizeof(struct ffly_vec_blkd));
    mdl_uint_t page = blk->page;
    mdl_uint_t off = blk->off;
	while(ffly_is_flag(blk->flags, VEC_BLK_FREE)) {
        blk = (struct ffly_vec_blkd*)((mdl_u8_t*)blk+__vec->blk_size);
        if (off == VEC_PAGE_SIZE-1) {
            off = 0;
            blk = (struct ffly_vec_blkd*)*((void**)__vec->p+(++page));
        } else
            off++; 
    }
	return (void*)((mdl_u8_t*)blk+sizeof(struct ffly_vec_blkd));
}

void* ffly_vec_rend(ffly_vecp __vec) {
	struct ffly_vec_blkd *blk = (struct ffly_vec_blkd*)((mdl_u8_t*)ffly_vec_end(__vec)-sizeof(struct ffly_vec_blkd));
    mdl_uint_t page = blk->page;
    mdl_uint_t off = blk->off;
	while(ffly_is_flag(blk->flags, VEC_BLK_FREE)) {
        blk = (struct ffly_vec_blkd*)((mdl_u8_t*)blk-__vec->blk_size);
        if (!off) {
            off = VEC_PAGE_SIZE-1;
            blk = (struct ffly_vec_blkd*)(((mdl_u8_t*)*((void**)__vec->p+(--page)))+((VEC_PAGE_SIZE-1)*__vec->blk_size)); 
        } else
            off--; 
    }
	return (void*)((mdl_u8_t*)blk+sizeof(struct ffly_vec_blkd));
}

void* ffly_vec_first(ffly_vecp __vec) {
    if (is_flag(__vec, VEC_NONCONTINUOUS))
        return (void*)((mdl_u8_t*)get_at(__vec, __vec->top)+sizeof(struct ffly_vec_blkd));

	return (void*)((mdl_u8_t*)__vec->p+sizeof(struct ffly_vec_blkd)+(__vec->top*__vec->blk_size));}

void* ffly_vec_last(ffly_vecp __vec) {
    if (is_flag(__vec, VEC_NONCONTINUOUS))
        return (void*)((mdl_u8_t*)get_at(__vec, __vec->end)+sizeof(struct ffly_vec_blkd));

	return (void*)((mdl_u8_t*)__vec->p+sizeof(struct ffly_vec_blkd)+(__vec->end*__vec->blk_size));}

void ffly_vec_dechain(ffly_vecp __vec, struct ffly_vec_blkd *__blk) {
	ffly_vec_blkdp prev;
	ffly_vec_blkdp next;
    if (is_flag(__vec, VEC_NONCONTINUOUS)) {
        if (not_null(__blk->prev))
            prev = (ffly_vec_blkdp)get_at(__vec, __blk->prev);
        if (not_null(__blk->next))
            next = (ffly_vec_blkdp)get_at(__vec, __blk->next);

    } else {
        prev = (ffly_vec_blkdp)((mdl_u8_t*)__vec->p+__blk->prev);
        next = (ffly_vec_blkdp)((mdl_u8_t*)__vec->p+__blk->next);
    }

    ffly_off_t off = (__blk->page*VEC_PAGE_SIZE)+__blk->off;
	if (not_null(__vec->top) && off == __vec->top)
		__vec->top = __blk->next;
	if (not_null(__vec->end) && off == __vec->end)
		__vec->end = __blk->prev;

	if (not_null(__blk->prev))
		prev->next = __blk->next;	
	if (not_null(__blk->next))
		next->prev = __blk->prev;
    __blk->next = VEC_NULL;
    __blk->prev = VEC_NULL;
}

void ffly_vec_del(ffly_vecp __vec, void *__p) {
	ffly_vec_blkdp blk = (ffly_vec_blkdp)((mdl_u8_t*)__p-sizeof(struct ffly_vec_blkd));

	ffly_rm_flag(&blk->flags, VEC_BLK_USED);
	ffly_add_flag(&blk->flags, VEC_BLK_FREE, 0);
	ffly_vec_dechain(__vec, blk);

	ffly_off_t *p;
	ffly_vec_push_back(__vec->uu_blks, (void**)&p);
	*p = (blk->page*VEC_PAGE_SIZE)+blk->off;
    __vec->size--;
}

ffly_err_t ffly_vec_push_back(ffly_vecp __vec, void **__p) {
	__vec->size++;
	if (is_flag(__vec, VEC_BLK_CHAIN) && is_flag(__vec, VEC_UUU_BLKS)) {
		if (ffly_vec_size(__vec->uu_blks) > 0) {
			ffly_off_t off;
			ffly_vec_pop_back(__vec->uu_blks, (void*)&off);
			ffly_vec_blkdp blk = (ffly_vec_blkdp)(is_flag(__vec, VEC_NONCONTINUOUS)?get_at(__vec, off):((mdl_u8_t*)__vec->p+(off*__vec->blk_size)));
            blk->flags = VEC_BLK_USED;
            blk->prev = __vec->end;
            blk->next = VEC_NULL;

            if (is_flag(__vec, VEC_NONCONTINUOUS))
                ((ffly_vec_blkdp)get_at(__vec, __vec->end))->next = off;
            else
			    ((ffly_vec_blkdp)((mdl_u8_t*)__vec->p+__vec->end))->next = off;
			*__p = (void*)((mdl_u8_t*)blk+sizeof(struct ffly_vec_blkd));
			__vec->end = off;
			return FFLY_SUCCESS;
		}
	}

	if (is_flag(__vec, VEC_AUTO_RESIZE)) {
		if (__vec->off>>VEC_PAGE_SHIFT >= __vec->page_c) {
            if (is_flag(__vec, VEC_NONCONTINUOUS)) {
                __vec->p = __ffly_mem_realloc(__vec->p, (++__vec->page_c)*sizeof(void*));
                *((void**)__vec->p+(__vec->page_c-1)) = __ffly_mem_alloc(VEC_PAGE_SIZE*__vec->blk_size);
            } else {
			    if ((__vec->p = __ffly_mem_realloc(__vec->p, (++__vec->page_c)*(VEC_PAGE_SIZE*__vec->blk_size))) == NULL) {
				    ffly_fprintf(ffly_err, "vec: failed to realloc memory for next page, errno: %d.\n", errno);
				    return FFLY_FAILURE;
			    }
            }
		}
	}

    ffly_off_t off = __vec->off*__vec->blk_size;
    ffly_off_t blk_off = __vec->off;
    if (is_flag(__vec, VEC_NONCONTINUOUS)) {
        *__p = get_at(__vec, blk_off);
    } else
	    *__p = (void*)((mdl_u8_t*)__vec->p+off);

	if (is_flag(__vec, VEC_BLK_CHAIN)) {
		ffly_vec_blkdp blk = (ffly_vec_blkdp)((mdl_u8_t*)*__p);
		*__p = (void*)(((mdl_u8_t*)*__p)+sizeof(struct ffly_vec_blkd));
		blk->prev = VEC_NULL;
        blk->next = VEC_NULL;
		blk->flags = VEC_BLK_USED;
        blk->page = blk_off>>VEC_PAGE_SHIFT;
        blk->off = blk_off-(blk->page*VEC_PAGE_SIZE);
		if (not_null(__vec->end)) {
			blk->prev = __vec->end;
            if (is_flag(__vec, VEC_NONCONTINUOUS))
                ((ffly_vec_blkdp)get_at(__vec, __vec->end))->next = blk_off;
            else
			    ((ffly_vec_blkdp)((mdl_u8_t*)__vec->p+(__vec->end*__vec->blk_size)))->next = blk_off;

		}
		if (!not_null(__vec->top))
			__vec->top = blk_off;
        __vec->end = blk_off;
	}
	__vec->off++;
	return FFLY_SUCCESS;
}
ffly_err_t ffly_vec_pop_back(ffly_vecp __vec, void *__p) {
	__vec->size--;
	if (is_flag(__vec, VEC_AUTO_RESIZE)) {
		if (__vec->off>>VEC_PAGE_SHIFT < __vec->page_c-1 && __vec->page_c > 0) {
            if (is_flag(__vec, VEC_NONCONTINUOUS)) { 
                void *page = *((void**)__vec->p+(--__vec->page_c));
                __ffly_mem_free(page);
                __vec->p = __ffly_mem_realloc(__vec->p, __vec->page_c*sizeof(void*)); 
            } else {
			    if ((__vec->p = __ffly_mem_realloc(__vec->p, (--__vec->page_c)*(VEC_PAGE_SIZE*__vec->blk_size))) == NULL) {
				    ffly_fprintf(ffly_err, "vec: failed to realloc memory for next page, errno: %d.\n", errno);
			    	return FFLY_FAILURE;
			    }
            }
		}
	}

	if (is_flag(__vec, VEC_BLK_CHAIN)) {
		void *last = ffly_vec_last(__vec);
		ffly_vec_blkdp blk = (ffly_vec_blkdp)((mdl_u8_t*)last-sizeof(struct ffly_vec_blkd));
		ffly_mem_cpy(__p, last, __vec->blk_size-sizeof(struct ffly_vec_blkd));
		if (is_flag(__vec, VEC_NONCONTINUOUS)?(blk == get_at(__vec, __vec->off-1)):(blk == (void*)((mdl_u8_t*)__vec->p+((__vec->off-1)*__vec->blk_size)))) {
			ffly_vec_dechain(__vec, blk);
			__vec->off--;
		} else {
		    ffly_printf("--> %u, %p-%p, %u, %u\n", __vec->off, last, (void*)((mdl_u8_t*)__vec->p+((__vec->off-1)*__vec->blk_size)), __vec->top, __vec->end);
            ffly_vec_del(__vec, last);
        }
	} else {
        if (is_flag(__vec, VEC_NONCONTINUOUS))
            ffly_mem_cpy(__p, get_at(__vec, __vec->off-1), __vec->blk_size);
        else
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
    if (is_flag(__vec, VEC_NONCONTINUOUS)) {
        void **page = (void**)__vec->p;
        while(page != (void**)__vec->p+__vec->page_c) {
            __ffly_mem_free(*page);
            page++;
        }
    }

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
