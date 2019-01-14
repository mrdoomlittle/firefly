# include "../ffint.h"
# include "vec.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# include "io.h"
# include "../dep/mem_cpy.h"
# include "errno.h"
# include "../oddity.h"
#define VEC_NULL ((ff_off_t)~0)
#define VEC_BLK_USED 0x01
#define VEC_BLK_FREE 0x02
#define is_flag(__vec, __flag) ffly_is_flag(__vec->flags, __flag)
#define not_null(__v)(__v!=VEC_NULL)

#define get_at(__vec, __off) \
	((void*)(((ff_u8_t*)*((void**)__vec->p+(__off>>VEC_PAGE_SHIFT)))+(((__off)-(((__off)>>VEC_PAGE_SHIFT)<<VEC_PAGE_SHIFT))*__vec->blk_size)))
/*
void static get_at(ffly_vecp __vec, ff_off_t __off) {
	ff_uint_t pg, pg_off;
	pg = __off>>VEC_PAGE_SHIFT;
	pg_off = __off-(pg<<VEC_PAGE_SHIFT);
	return ((ff_u8_t*)*((void**)(__vec->p+pg)))+(pg_off*__vec->blk_size);
}
*/

#ifndef FF_VEC_SA
#define mem_alloc(__n)\
    __ffly_mem_alloc(__n)   
#define mem_free(__p)\
    __ffly_mem_free(__p)
#define mem_realloc(__p, __n)\
    __ffly_mem_realloc(__p, __n)
#else
#define mem_alloc(__n)\
    __vec->alloc(__vec->ma_arg, __n)
#define mem_free(__p)\
    __vec->free(__vec->ma_arg, __p)
#define mem_realloc(__p, __n)\
    __vec->realloc(__vec->ma_arg, __p, __n)
#endif

ffly_vecp static top = NULL;
ffly_vecp ffly_vec_list(void) {
	return top;
}

/*
	TODO:
		mega cleanup, i am not joking it bad
*/

// later
struct page {
	void **p;

};

void ffly_vec_attach(ffly_vecp __vec) {
	__vec->prev = NULL;
	__vec->next = top;
	if (top != NULL)
		top->prev = __vec;
	top = __vec;
}

void ffly_vec_detach(ffly_vecp __vec) {
	if (__vec == top) {
		if ((top = __vec->next) != NULL)
			top->prev = NULL;
		return;
	} 

	if (__vec->prev != NULL)
		__vec->prev->next = __vec->next;
	if (__vec->next != NULL)
		__vec->next->prev = __vec->prev;
}

/*
	just incase user is a idiot and does not set stuff up right 
*/
void static* dummy_alloc(long long __arg, ff_uint_t __n) {
	return __ffly_mem_alloc(__n);
}
void static dummy_free(long long __arg, void *__p) {
    __ffly_mem_free(__p);
}
void static* dummy_realloc(long long __arg, void *__p, ff_uint_t __n) {
    return __ffly_mem_realloc(__p, __n);
}

ffly_vecp _ffly_vec_creat(ff_size_t __blk_size, ff_flag_t __flags, ff_err_t *__err, void(*__grab)(long long, ff_u8_t)) {
	ffly_vecp p;
	void*(*alloc)(long long, ff_uint_t);
#ifdef FF_VEC_SA
	if (!__grab) {
		alloc = dummy_alloc;	
		goto _sk;
	}
	__grab((long long)&alloc, FF_VGF_MA);
#else
	alloc = dummy_alloc;
#endif
_sk:
	p = (ffly_vecp)alloc(~0, sizeof(struct ffly_vec));
	p->flags = __flags;
	*__err = _ffly_vec_init(p, __blk_size, __grab);
	return p;
}

ffly_vecp _ffly_vec(ff_size_t __blk_size, ff_flag_t __flags, ff_err_t *__err, void(*__grab)(long long, ff_u8_t)) { 
	ffly_vecp p;
	void*(*alloc)(long long, ff_uint_t);
#ifdef FF_VEC_SA
	if (!__grab) {
		alloc = dummy_alloc;
		goto _sk;
	}
	// get allocation function
	__grab((long long)&alloc, FF_VGF_MA);
#else
	alloc = dummy_alloc;
#endif
_sk:
	p = (ffly_vecp)alloc(~0, sizeof(struct ffly_vec));
	p->flags = __flags;
	*__err = _ffly_vec_init(p, __blk_size, __grab);
	ffly_vec_attach(p);
	return p;
}

ff_err_t ffly_vec_destroy(ffly_vecp __vec) {
	ffly_vec_de_init(__vec);
	ffly_vec_free(__vec);
}

ff_err_t ffly_vec_free(ffly_vecp __vec) {
	ffly_vec_detach(__vec);
	__ffly_mem_free(__vec);
}

#define vec_off(__vec, __p)\
	((ff_u8_t*)__p-(ff_u8_t*)__vec->p)

ff_off_t ffly_vec_off(ffly_vecp __vec, void *__p) {
	return (ff_u8_t*)__p-(ff_u8_t*)__vec->p;
}

void* ffly_vec_at(ffly_vecp __vec, ff_uint_t __off) {
	return (void*)((ff_u8_t*)__vec->p+(__off*__vec->blk_size));
}

ff_uint_t ffly_vec_blk_off(ffly_vecp __vec, void *__p) {
	ff_off_t off;
	off = ffly_vec_off(__vec, __p);
	if (is_flag(__vec, VEC_BLK_CHAIN))
		return (off-sizeof(struct ffly_vec_blkd))/__vec->blk_size;
	return off/__vec->blk_size;
}

void* ffly_vec_begin(ffly_vecp __vec) {
	void *p;
	
	p = is_flag(__vec, VEC_NONCONTINUOUS)?(*(void**)__vec->p):__vec->p;
	if (is_flag(__vec, VEC_BLK_CHAIN))
		p = (ff_u8_t*)p+sizeof(struct ffly_vec_blkd);
	return p;
}

void* ffly_vec_end(ffly_vecp __vec) {
	void *p;
	if (is_flag(__vec, VEC_NONCONTINUOUS)) {
		ff_uint_t off = __vec->off-1;
		ff_uint_t page = off>>VEC_PAGE_SHIFT;
		p = ((ff_u8_t*)*((void**)__vec->p+page))+((off-(page*VEC_PAGE_SIZE))*__vec->blk_size);   
	} else
		p = (void*)((ff_u8_t*)__vec->p+((__vec->off-1)*__vec->blk_size));
	if (is_flag(__vec, VEC_BLK_CHAIN))
		p = (ff_u8_t*)p+sizeof(struct ffly_vec_blkd);
	return p;
}

ff_err_t _ffly_vec_init(ffly_vecp __vec, ff_size_t __blk_size, void(*__grab)(long long, ff_u8_t)) {
#ifdef FF_VEC_SA
	if (!is_flag(__vec, VEC_AS) && __grab != NULL) {
		__grab((long long)__vec->alloc, FF_VGF_MA);
		__grab((long long)__vec->free, FF_VGF_MF);
		__grab((long long)__vec->realloc, FF_VGF_MR);
		goto _sk;
	}

	__vec->alloc = dummy_alloc;
	__vec->free = dummy_free;
	__vec->realloc = dummy_realloc;
_sk:
#endif
	__vec->grab = __grab;
	__vec->p = NULL;
	__vec->page_c = 0;

	__vec->blk_size = __blk_size;
	// add block header to blksize
	if (is_flag(__vec, VEC_BLK_CHAIN))
		__vec->blk_size+=sizeof(struct ffly_vec_blkd);

	if (is_flag(__vec, VEC_AUTO_RESIZE)) {
		if (is_flag(__vec, VEC_NONCONTINUOUS)) {
			if ((__vec->p = mem_alloc(sizeof(void*))) == NULL) {
				return FFLY_FAILURE;
			}

			*(void**)__vec->p = mem_alloc(VEC_PAGE_SIZE*__vec->blk_size);
		} else {
			if ((__vec->p = mem_alloc(VEC_PAGE_SIZE*__vec->blk_size)) == NULL) {
				ffly_fprintf(ffly_err, "vec: failed to alloc memory, errno: %d.\n", 0);
				caught_oddity;
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
		__vec->uu_blks = (ffly_vecp)mem_alloc(sizeof(struct ffly_vec));
		__vec->uu_blks->flags = VEC_AUTO_RESIZE;
		if (_ffly_vec_init(__vec->uu_blks, sizeof(ff_off_t), __grab) != FFLY_SUCCESS) {
			ffly_fprintf(ffly_err, "vec: failed to init uu_blks->\n");
			caught_oddity;
			return FFLY_FAILURE;
		}
	} else
		__vec->uu_blks = NULL;
	return FFLY_SUCCESS;
}

void ffly_vec_fd(ffly_vecp __vec, void **__p) {
	struct ffly_vec_blkd *blk = (struct ffly_vec_blkd*)(((ff_u8_t*)*__p)-sizeof(struct ffly_vec_blkd));
	if (!not_null(blk->next)) {*__p = NULL;return;}
	if (is_flag(__vec, VEC_NONCONTINUOUS))
		*__p = (void*)((ff_u8_t*)get_at(__vec, blk->next)+sizeof(struct ffly_vec_blkd));
	else
		*__p = (void*)(((ff_u8_t*)__vec->p+sizeof(struct ffly_vec_blkd))+(blk->next*__vec->blk_size));
}

void ffly_vec_bk(ffly_vecp __vec,  void **__p) {
	struct ffly_vec_blkd *blk;
	
	blk = (struct ffly_vec_blkd*)(((ff_u8_t*)*__p)-sizeof(struct ffly_vec_blkd));

	if (!not_null(blk->prev)) {*__p = NULL;return;}
	if (is_flag(__vec, VEC_NONCONTINUOUS))
		*__p = (void*)((ff_u8_t*)get_at(__vec, blk->prev)+sizeof(struct ffly_vec_blkd));
	else
		*__p = (void*)(((ff_u8_t*)__vec->p+sizeof(struct ffly_vec_blkd))+(blk->prev*__vec->blk_size));
}

void ffly_vec_itr(ffly_vecp __vec, void **__p, ff_u8_t __dir, ff_uint_t __ia) {
	while(__ia != 0) {
		if (__dir == VEC_ITR_FD)
			ffly_vec_fd(__vec, __p);
		else if (__dir == VEC_ITR_BK)
			ffly_vec_bk(__vec, __p);
		if (!*__p) return;
		__ia--;
	}
}

void* ffly_vec_rbegin(ffly_vecp __vec) {
	struct ffly_vec_blkd *blk;
	
	blk = (struct ffly_vec_blkd*)((ff_u8_t*)ffly_vec_begin(__vec)-sizeof(struct ffly_vec_blkd));
	ff_uint_t page = blk->page;
	ff_uint_t off = blk->off;
	while(ffly_is_flag(blk->flags, VEC_BLK_FREE)) {
		blk = (struct ffly_vec_blkd*)((ff_u8_t*)blk+__vec->blk_size);
		if (off == VEC_PAGE_SIZE-1) {
			off = 0;
			blk = (struct ffly_vec_blkd*)*((void**)__vec->p+(++page));
		} else
			off++; 
	}
	return (void*)((ff_u8_t*)blk+sizeof(struct ffly_vec_blkd));
}

void* ffly_vec_rend(ffly_vecp __vec) {
	struct ffly_vec_blkd *blk = (struct ffly_vec_blkd*)((ff_u8_t*)ffly_vec_end(__vec)-sizeof(struct ffly_vec_blkd));
	ff_uint_t page = blk->page;
	ff_uint_t off = blk->off;
	while(ffly_is_flag(blk->flags, VEC_BLK_FREE)) {
		blk = (struct ffly_vec_blkd*)((ff_u8_t*)blk-__vec->blk_size);
		if (!off) {
			off = VEC_PAGE_SIZE-1;
			blk = (struct ffly_vec_blkd*)(((ff_u8_t*)*((void**)__vec->p+(--page)))+((VEC_PAGE_SIZE-1)*__vec->blk_size)); 
		} else
			off--; 
	}
	return (void*)((ff_u8_t*)blk+sizeof(struct ffly_vec_blkd));
}

void* ffly_vec_first(ffly_vecp __vec) {
	if (is_flag(__vec, VEC_NONCONTINUOUS))
		return (void*)((ff_u8_t*)get_at(__vec, __vec->top)+sizeof(struct ffly_vec_blkd));

	return (void*)((ff_u8_t*)__vec->p+sizeof(struct ffly_vec_blkd)+(__vec->top*__vec->blk_size));}

void* ffly_vec_last(ffly_vecp __vec) {
	if (is_flag(__vec, VEC_NONCONTINUOUS))
		return (void*)((ff_u8_t*)get_at(__vec, __vec->end)+sizeof(struct ffly_vec_blkd));

	return (void*)((ff_u8_t*)__vec->p+sizeof(struct ffly_vec_blkd)+(__vec->end*__vec->blk_size));}

void ffly_vec_dechain(ffly_vecp __vec, struct ffly_vec_blkd *__blk) {
	ffly_vec_blkdp prev;
	ffly_vec_blkdp next;

	// get prev&next
	if (is_flag(__vec, VEC_NONCONTINUOUS)) {
		if (not_null(__blk->prev))
			prev = (ffly_vec_blkdp)get_at(__vec, __blk->prev);
		if (not_null(__blk->next))
			next = (ffly_vec_blkdp)get_at(__vec, __blk->next);
	} else {
		prev = (ffly_vec_blkdp)((ff_u8_t*)__vec->p+__blk->prev);
		next = (ffly_vec_blkdp)((ff_u8_t*)__vec->p+__blk->next);
	}

	ff_off_t off = (__blk->page*VEC_PAGE_SIZE)+__blk->off;
	if (not_null(__vec->top) && off == __vec->top) {
		if (not_null(__vec->top = __blk->next))
			next->prev = VEC_NULL;
		goto _end;
	}

	if (not_null(__vec->end) && off == __vec->end) {
		if (not_null(__vec->end = __blk->prev))
			prev->next = VEC_NULL;
		goto _end;
	}

	if (not_null(__blk->prev))
		prev->next = __blk->next;	
	if (not_null(__blk->next))
		next->prev = __blk->prev;
_end:
	__blk->next = VEC_NULL;
	__blk->prev = VEC_NULL;
}

void ffly_vec_del(ffly_vecp __vec, void *__p) {
	ffly_vec_blkdp blk = (ffly_vec_blkdp)((ff_u8_t*)__p-sizeof(struct ffly_vec_blkd));

	ffly_rm_flag(blk->flags, VEC_BLK_USED);
	ffly_add_flag(blk->flags, VEC_BLK_FREE);
	ffly_vec_dechain(__vec, blk);

	ff_off_t *p;
	ffly_vec_push_back(__vec->uu_blks, (void**)&p);
	*p = (blk->page*VEC_PAGE_SIZE)+blk->off;
	__vec->size--;
}

ff_err_t ffly_vec_push_back(ffly_vecp __vec, void **__p) {
	__vec->size++;
	if (is_flag(__vec, VEC_BLK_CHAIN) && is_flag(__vec, VEC_UUU_BLKS)) {
		/*
			check for unused blocks
		*/
		if (ffly_vec_size(__vec->uu_blks)>0) {
			ff_off_t off;
			ffly_vec_pop_back(__vec->uu_blks, (void*)&off);
			ffly_vec_blkdp blk;
			
			if (is_flag(__vec, VEC_NONCONTINUOUS))
				blk = (ffly_vec_blkdp)get_at(__vec, off);
			else
				blk = (ffly_vec_blkdp)((ff_u8_t*)__vec->p+(off*__vec->blk_size));

			blk->flags = VEC_BLK_USED;
			blk->prev = __vec->end;
			blk->next = VEC_NULL;

			if (is_flag(__vec, VEC_NONCONTINUOUS))
				((ffly_vec_blkdp)get_at(__vec, __vec->end))->next = off;
			else
				((ffly_vec_blkdp)((ff_u8_t*)__vec->p+__vec->end))->next = off;
			*__p = (void*)((ff_u8_t*)blk+sizeof(struct ffly_vec_blkd));
			__vec->end = off;
			return FFLY_SUCCESS;
		}
	}

	if (is_flag(__vec, VEC_AUTO_RESIZE)) {
		if (__vec->off>>VEC_PAGE_SHIFT >= __vec->page_c) {
			if (is_flag(__vec, VEC_NONCONTINUOUS)) {
				__vec->p = mem_realloc(__vec->p, (++__vec->page_c)*sizeof(void*));
				*((void**)__vec->p+(__vec->page_c-1)) = mem_alloc(VEC_PAGE_SIZE*__vec->blk_size);
			} else {
				if ((__vec->p = mem_realloc(__vec->p, (++__vec->page_c)*(VEC_PAGE_SIZE*__vec->blk_size))) == NULL) {
					ffly_fprintf(ffly_err, "vec: failed to realloc memory for next page, errno: %d.\n", 0);
					caught_oddity;
					return FFLY_FAILURE;
				}
			}
		}
	}

	ff_off_t off = __vec->off*__vec->blk_size;
	ff_off_t blk_off = __vec->off;
	if (is_flag(__vec, VEC_NONCONTINUOUS)) {
		*__p = get_at(__vec, blk_off);
	} else
		*__p = (void*)((ff_u8_t*)__vec->p+off);

	if (is_flag(__vec, VEC_BLK_CHAIN)) {
		ffly_vec_blkdp blk = (ffly_vec_blkdp)((ff_u8_t*)*__p);
		*__p = (void*)(((ff_u8_t*)*__p)+sizeof(struct ffly_vec_blkd));
		blk->prev = VEC_NULL;
		blk->next = VEC_NULL;
		blk->flags = VEC_BLK_USED;
		blk->page = blk_off>>VEC_PAGE_SHIFT;
		blk->off = blk_off-(blk->page<<VEC_PAGE_SHIFT);
		if (not_null(__vec->end)) {
			blk->prev = __vec->end;
			if (is_flag(__vec, VEC_NONCONTINUOUS))
				((ffly_vec_blkdp)get_at(__vec, __vec->end))->next = blk_off;
			else
				((ffly_vec_blkdp)((ff_u8_t*)__vec->p+(__vec->end*__vec->blk_size)))->next = blk_off;

		}
		if (!not_null(__vec->top))
			__vec->top = blk_off;
		__vec->end = blk_off;
	}
	__vec->off++;
	return FFLY_SUCCESS;
}

ff_err_t ffly_vec_pop_back(ffly_vecp __vec, void *__p) {
	__vec->size--;
	ff_uint_t page = __vec->off>>VEC_PAGE_SHIFT;
	if (is_flag(__vec, VEC_AUTO_RESIZE)) {
		if (page < __vec->page_c-1 && __vec->page_c>1) {
			if (is_flag(__vec, VEC_NONCONTINUOUS)) { 
				void *page = *((void**)__vec->p+(--__vec->page_c));
				mem_free(page);
				__vec->p = mem_realloc(__vec->p, __vec->page_c*sizeof(void*)); 
			} else {
				if ((__vec->p = mem_realloc(__vec->p, (--__vec->page_c)*(VEC_PAGE_SIZE*__vec->blk_size))) == NULL) {
					ffly_fprintf(ffly_err, "vec: failed to realloc memory for next page, errno: %d.\n", 0);
					caught_oddity;
					return FFLY_FAILURE;
				}
			}
		}
	}

	if (is_flag(__vec, VEC_BLK_CHAIN)) {
		void *last = ffly_vec_last(__vec);
		ff_uint_t size = __vec->blk_size-sizeof(struct ffly_vec_blkd);
		ffly_vec_blkdp blk = (ffly_vec_blkdp)((ff_u8_t*)last-sizeof(struct ffly_vec_blkd));
		ffly_mem_cpy(__p, last, size);
		
		/*
			check if block matches end block
		*/
		ff_u8_t co;
		if (is_flag(__vec, VEC_NONCONTINUOUS))
			co = ((void*)blk == get_at(__vec, __vec->off-1));
		else
			co = ((void*)blk == (void*)((ff_u8_t*)__vec->p+((__vec->off-1)*__vec->blk_size)));

		/*
			if block is at the end, then remove it else delete it.
			delete wont remove block but put it with all the other unused blocks so push can take a pick from the pool of them
		*/
		if (co) {
			ffly_vec_dechain(__vec, blk);
			__vec->off--;
		} else {
			ffly_printf("--> %u, %p-%p, %u, %u\n", __vec->off, last, (void*)((ff_u8_t*)__vec->p+((__vec->off-1)*__vec->blk_size)), __vec->top, __vec->end);
			ffly_vec_del(__vec, last);
		}
	} else {
		ff_uint_t bks;
		bks = __vec->blk_size;
		/*
			copy memory to user
		*/
		if (is_flag(__vec, VEC_NONCONTINUOUS))
			ffly_mem_cpy(__p, get_at(__vec, __vec->off-1), bks);
		else
			ffly_mem_cpy(__p, (void*)((ff_u8_t*)__vec->p+((__vec->off-1)*bks)), bks);
		__vec->off--;
	}
	return FFLY_SUCCESS;
}

/*
	only works without VEC_NONCONTINUOUS
*/
ff_err_t ffly_vec_resize(ffly_vecp __vec, ff_size_t __size) {
	__vec->size = __size;
	if (!__size) {
		if (__vec->p != NULL) {
			mem_free(__vec->p);
			__vec->p = NULL;
		}
		return FFLY_SUCCESS;	
	}

	if (!__vec->p) {
		__vec->p = mem_alloc(__size*__vec->blk_size);
		return FFLY_SUCCESS;
	}

	if ((__vec->p = mem_realloc(__vec->p, __size*__vec->blk_size)) == NULL) {
		ffly_fprintf(ffly_err, "vec: failed to realloc memory for resize, errno: %d\n", 0);
		caught_oddity;
		return FFLY_FAILURE;
	}
	ffly_fprintf(ffly_log, "vec, resize.\n");
	return FFLY_SUCCESS;
}

ff_err_t ffly_vec_de_init(ffly_vecp __vec) {
	if (is_flag(__vec, VEC_NONCONTINUOUS)) {
		/*
			free all pages
		*/
		void **page = (void**)__vec->p;
		void **end;
		end = page+__vec->page_c;
		while(page != end) {
			if (*page != NULL)	
				mem_free(*page);
			else {
				// something is wrong
				caught_oddity;
			}
			page++;
		}
	}

	if (__vec->p != NULL) {
		mem_free(__vec->p);
		__vec->p = NULL;
	}

	if (__vec->uu_blks != NULL) {
		ffly_vec_de_init(__vec->uu_blks);
		__ffly_mem_free(__vec->uu_blks);
		__vec->uu_blks = NULL;
	}
	return FFLY_SUCCESS;
}
