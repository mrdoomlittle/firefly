# include "map.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# include "util/hash.h"
# include "../dep/mem_cmp.h"
# include "../dep/mem_dup.h"
# include "errno.h"
# include "err.h"
# include "io.h"
# include "../oddity.h"
#define __MFSPS __MFSPS
#define __VFSPS __VFSPS
#define map_mask(__MFSPS) ((~(ff_u64_t)0)>>(64-__MFSPS->size))
#define PAGE_SHIFT 4
#define PAGE_SIZE (1<<PAGE_SHIFT)

#ifndef FF_MAP_SA
#define mem_alloc(__n)\
	__ffly_mem_alloc(__n)   
#define mem_free(__p)\
	__ffly_mem_free(__p)
#define mem_realloc(__p, __n)\
	__ffly_mem_realloc(__p, __n)
#else
#ifdef VEC_STUFF
#define mem_alloc(__n)\
	__VFSPS->maf->alloc(*__VFSPS->ma_arg, __n)
#define mem_free(__p)\
	__VFSPS->maf->free(*__VFSPS->ma_arg, __p)
#define mem_realloc(__p, __n)\
	__VFSPS->maf->realloc(*__VFSPS->ma_arg, __p, __n)
#else
#define mem_alloc(__n)\
	__MFSPS->maf.alloc(__MFSPS->ma_arg, __n)
#define mem_free(__p)\
	__MFSPS->maf.free(__MFSPS->ma_arg, __p)
#define mem_realloc( __p, __n)\
	__MFSPS->maf.realloc(__MFSPS->ma_arg, __p, __n)
#endif
#endif

/*
	TODO:
		remove any trace of vec and use pages of pointers
*/
/*still working on this*/
typedef struct {
	ff_u64_t val;
	ff_u8_t const *key;
	ff_uint_t bc;
	void *p;
	void *prev, *next;
	struct vec *blk;
} map_entry_t;

#define vec_fbegin(__vec)\
	((__vec)->face)
#define vec_fend(__vec)\
	((__vec)->face+(__vec)->size-1)
#define vec_size(__vec)\
	((__vec)->size)
struct vec_blk;
typedef struct vec {
	void **p;
	ff_uint_t page_c;
	ff_uint_t off;
	ff_uint_t bks;
	ff_uint_t size;
	void **face;
	struct vec_blk *uu;
	struct ffly_map_maf *maf;
	long long *ma_arg;
} *vecp;

typedef struct vec_blk {
	ff_uint_t off;
	ff_i8_t inuse;
	ff_uint_t n;
	struct vec_blk **bk, *fd;
} *vec_blkp;

ff_err_t static vec_init(vecp __VFSPS, ff_uint_t __blk_size, struct ffly_map_maf *__af, long long *__ma_arg) {
	__VFSPS->bks = __blk_size+sizeof(struct vec_blk);
	__VFSPS->p = NULL;
	__VFSPS->page_c = 0;
	__VFSPS->off = 0;
	__VFSPS->uu = NULL;
	__VFSPS->size = 0;
	__VFSPS->face = NULL;
	__VFSPS->maf = __af;
	__VFSPS->ma_arg = __ma_arg;
	return 0;
}
#define VEC_STUFF
ff_err_t static vec_de_init(vecp __VFSPS) {
	if (__VFSPS->p != NULL) {
		void **pg, **end, *p;
		pg = __VFSPS->p;
		end = pg+__VFSPS->page_c;
		while(pg != end) {
			if ((p = *pg) != NULL) {
				mem_free(p);
			} else {
				// something is wrong
				caught_oddity;
			}
			pg++;
		}
	}
	mem_free(__VFSPS->p);
	return 0;
}

ff_err_t static vec_push_back(vecp __VFSPS, void **__p) {
	__VFSPS->size++;
	if (!__VFSPS->face) {
		__VFSPS->face = (void**)mem_alloc(__VFSPS->size*sizeof(void*));
	} else {
		__VFSPS->face = (void**)mem_realloc(__VFSPS->face, __VFSPS->size*sizeof(void*));
	}

	void **fp;

	fp = __VFSPS->face+(__VFSPS->size-1);

	if (__VFSPS->uu != NULL) {
		vec_blkp b;
		b = __VFSPS->uu;
		__VFSPS->uu = b->fd;
		b->fd->bk = b->bk;
		b->bk = NULL;
		b->fd = NULL;
		b->n = __VFSPS->size-1;
		b->inuse = 0;
		*fp = *__p = ((ff_u8_t*)b)+sizeof(struct vec_blk);
		return 0;
	}

	ff_uint_t pg, pg_off;
	pg = __VFSPS->off>>PAGE_SHIFT;
	pg_off = __VFSPS->off-(pg<<PAGE_SHIFT);
	if (pg>=__VFSPS->page_c) {
		ff_uint_t pgc;
		pgc = ++__VFSPS->page_c;
		if (!__VFSPS->p) {
			__VFSPS->p = (void**)mem_alloc(pgc*sizeof(void*));
		} else {
			__VFSPS->p = (void**)mem_realloc(__VFSPS->p, pgc*sizeof(void*));
		}
		*(__VFSPS->p+pg) = mem_alloc(PAGE_SIZE*__VFSPS->bks);
	}
	ff_u8_t *p;
	p = ((ff_u8_t*)*(__VFSPS->p+pg))+(pg_off*__VFSPS->bks);
	
	vec_blkp b;
	b = (vec_blkp)p;
	b->bk = NULL;
	b->fd = NULL;
	b->off = __VFSPS->off;
	b->inuse = 0;
	b->n = __VFSPS->size-1;
	*fp = *__p = (void*)(p+sizeof(struct vec_blk));
	__VFSPS->off++;
	return 0;
}

ff_err_t static vec_pop_back(vecp __VFSPS, void *__p) {
	__VFSPS->size--;
	void *fe;

	fe = *(__VFSPS->face+__VFSPS->size);
	if (__VFSPS->size>1) {
		__VFSPS->face = (void**)mem_realloc(__VFSPS->face, __VFSPS->size*sizeof(void*));
	}

	ff_u8_t *p;
	p = (ff_u8_t*)__p;

	vec_blkp b;
	b = (vec_blkp)(p-sizeof(struct vec_blk));
	if (b->off == __VFSPS->off-1) {
		__VFSPS->off--;
		ff_uint_t pg;
		pg = __VFSPS->off>>PAGE_SHIFT;
		if (pg < __VFSPS->page_c-1 && __VFSPS->page_c>1) {
			__VFSPS->page_c--;
			mem_free(*(__VFSPS->p+pg));
			__VFSPS->p = (void**)mem_realloc(__VFSPS->p, __VFSPS->page_c*sizeof(void*));
		}
		return 0;
	}
	
	*(__VFSPS->face+b->n) = fe;
	if (__VFSPS->uu != NULL)
		__VFSPS->uu->bk = &b->fd;
	b->fd = __VFSPS->uu;
	__VFSPS->uu = b;
	b->bk = &__VFSPS->uu;
	b->inuse = -1;
	return 0;
}
#undef VEC_STUFF
void static* dummy_alloc(long long __arg, ff_uint_t __n) {
	return __ffly_mem_alloc(__n);
}
void static dummy_free(long long __arg, void *__p) {
	__ffly_mem_free(__p);
}
void static* dummy_realloc(long long __arg, void *__p, ff_uint_t __n) {
	return __ffly_mem_realloc(__p, __n);
}


ff_err_t _ffly_map_init(ffly_mapp __MFSPS, ff_uint_t __size, void(*__grab)(long long, ff_u8_t)) {
#ifdef FF_MAP_SA
	if (__grab != NULL) {
		__grab((long long)__MFSPS->alloc, FF_VGF_MA);
		__grab((long long)__MFSPS->free, FF_VGF_MF);
		__grab((long long)__MFSPS->realloc, FF_VGF_MR);
		goto _sk;
	}
	__MFSPS->maf.alloc = dummy_alloc;
	__MFSPS->maf.free	= dummy_free;
	__MFSPS->maf.realloc = dummy_realloc;
_sk:
#endif
	__MFSPS->size = __size;
	__MFSPS->table = mem_alloc((map_mask(__MFSPS)+1)*sizeof(struct vec*));
	struct vec **itr = (struct vec**)__MFSPS->table;
	while(itr != ((struct vec**)__MFSPS->table)+map_mask(__MFSPS)+1)
		*(itr++) = NULL;
	__MFSPS->begin = NULL;
	__MFSPS->end = NULL;
	__MFSPS->parent = NULL;
	return FFLY_SUCCESS;
}

ffly_mapp _ffly_map_creat(ff_uint_t __size, void(*__grab)(long long, ff_u8_t)) {
	ffly_mapp p;
	void*(*alloc)(long long, ff_uint_t);
#ifdef FF_MAP_SA
	if (!__grab) {
		
		goto _sk;
	}
	__grab((long long)&alloc, FF_MGF_MA);
#else
	alloc = dummy_alloc;
#endif
_sk:
	p = (ffly_mapp)alloc(~0, sizeof(struct ffly_map));
	_ffly_map_init(p, __size, __grab);
	return p;
}

ffly_mapp _ffly_map(ff_uint_t __size, void(*__grab)(long long, ff_u8_t)) {
	ffly_mapp p;

	p = (ffly_mapp)mem_alloc(sizeof(struct ffly_map));
	_ffly_map_init(p, __size, __grab);
	return p;
}

void ffly_map_free(ffly_mapp __MFSPS) {
	mem_free(__MFSPS);
}

void ffly_map_destroy(ffly_mapp __MFSPS) {
	ffly_map_de_init(__MFSPS);
	mem_free(__MFSPS);
}

void const* ffly_map_begin(ffly_mapp __MFSPS) {
	return (void const*)__MFSPS->begin;
}

void const* ffly_map_beg(ffly_mapp __MFSPS) {
	return (void const*)__MFSPS->begin;
}

void const* ffly_map_end(ffly_mapp __MFSPS) {
	return (void const*)__MFSPS->end;
}

void ffly_map_del(ffly_mapp __MFSPS, void const *__p) {
	map_entry_t *p = (map_entry_t*)__p;
	mem_free((void*)p->key);
	map_entry_t *end;
	vec_pop_back(p->blk, (void*)&end);
	*p = *end;
	mem_free(end);
}

void ffly_map_fd(ffly_mapp __MFSPS, void const **__p) {
	*(void**)__p = ((map_entry_t*)*__p)->next;
}

void ffly_map_bk(ffly_mapp __MFSPS, void const **__p) {
	*(void**)__p = ((map_entry_t*)*__p)->prev;
}

void ffly_map_itr(ffly_mapp __MFSPS, void const **__p, ff_u8_t __dir) {
	if (__dir == MAP_ITR_FD)
		ffly_map_fd(__MFSPS, __p);
	else if (__dir == MAP_ITR_BK)
		ffly_map_bk(__MFSPS, __p);	
}

void *const ffly_map_getp(void const *__p) {
	return ((map_entry_t*)__p)->p;
}

void ffly_map_parent(ffly_mapp __MFSPS, ffly_mapp __child) {
	__child->parent = __MFSPS;
}

map_entry_t static* map_find(ffly_mapp __MFSPS, ff_u8_t const *__key, ff_uint_t __bc) {
	ff_u64_t val = ffly_hash(__key, __bc);
	struct vec **blk = ((struct vec**)__MFSPS->table)+(val&map_mask(__MFSPS));
	if (!*blk) {
		ffly_fprintf(ffly_log, "map block is null, nothing hear.\n");
		return NULL;
	}

	void **itr;
	if (vec_size(*blk)>0) {
		itr = vec_fbegin(*blk);
		while(itr <= vec_fend(*blk)) {
			map_entry_t *entry = *(map_entry_t**)*itr;
			if (entry->val == val && entry->bc == __bc)
				if (!ffly_mem_cmp((void*)__key, (void*)entry->key, __bc)) return entry;
			itr++;
		}
	}

	if (__MFSPS->parent != NULL)
		return map_find(__MFSPS->parent, __key, __bc);
	ffly_fprintf(ffly_log, "diden't find.\n");
	return NULL;
}

ff_err_t ffly_map_put(ffly_mapp __MFSPS, ff_u8_t const *__key, ff_uint_t __bc, void *const __p) {
	ff_u64_t val = ffly_hash(__key, __bc);
	struct vec **blk = ((struct vec**)__MFSPS->table)+(val&map_mask(__MFSPS));
	if (!*blk) {
		*blk = (struct vec*)__ffly_mem_alloc(sizeof(struct vec));
		if (_err(vec_init(*blk, sizeof(map_entry_t*), &__MFSPS->maf, &__MFSPS->ma_arg))) {
			ffly_fprintf(ffly_err, "failed to init vec.\n");
		}
	}

	map_entry_t **entry;
	if (_err(vec_push_back(*blk, (void**)&entry))) {
		ffly_fprintf(ffly_err, "failed to push map entry.\n");
	}

	ff_u8_t *key;
	if (_err(ffly_mem_dupe((void**)&key, (void*)__key, __bc))) {
		ffly_fprintf(ffly_err, "failed to dupe key.\n");
	}

	*entry = (map_entry_t*)mem_alloc(sizeof(map_entry_t));
	**entry = (map_entry_t) {
		.val = val,
		.key = key,
		.bc = __bc,
		.p = __p,
		.next = NULL,
		.blk = *blk
	};

	if (__MFSPS->end != NULL) {
		((map_entry_t*)__MFSPS->end)->next = (void*)*entry;
		(*entry)->prev = __MFSPS->end;
		__MFSPS->end = (void*)*entry;
	} else
		__MFSPS->end = (void*)*entry;

	if (!__MFSPS->begin)
		__MFSPS->begin = (void*)*entry;
}

void const* ffly_map_fetch(ffly_mapp __MFSPS, ff_u8_t const *__key, ff_uint_t __bc) {
	return (void const*)map_find(__MFSPS, __key, __bc);
}

void *const ffly_map_get(ffly_mapp __MFSPS, ff_u8_t const *__key, ff_uint_t __bc, ff_err_t *__err) {
	map_entry_t *entry = map_find(__MFSPS, __key, __bc);
	if (!entry) {
		ffly_fprintf(ffly_log, "entry was not found.\n");
		*__err	= FFLY_FAILURE;
		return NULL;
	}
	*__err = FFLY_SUCCESS;
	return entry->p;
}

void static free_blk(struct vec **__blk) {
	void **itr;
	if (vec_size(*__blk)>0) {
		map_entry_t *et;
		itr = vec_fbegin(*__blk);
		while(itr <= vec_fend(*__blk)) {
			et = *(map_entry_t**)*itr;
			mem_free((void*)et->key);
			mem_free(et);
			itr++;
		}
	}
	vec_de_init(*__blk);
	mem_free(*__blk);
}

ff_err_t ffly_map_de_init(ffly_mapp __MFSPS) {
	struct vec **itr = (struct vec**)__MFSPS->table;
	while(itr != ((struct vec**)__MFSPS->table)+map_mask(__MFSPS)) {
		if (*itr != NULL) free_blk(itr);
		itr++;
	}
	mem_free(__MFSPS->table);
	return FFLY_SUCCESS;
}
