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
#define map_mask(__map) ((~(ff_u64_t)0)>>(64-__map->size))
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
	__vec->maf->alloc(*__vec->ma_arg, __n)
#define mem_free(__p)\
	__vec->maf->free(*__vec->ma_arg, __p)
#define mem_realloc(__p, __n)\
	__vec->maf->realloc(*__vec->ma_arg, __p, __n)
#else
#define mem_alloc(__n)\
	__map->maf.alloc(__map->ma_arg, __n)
#define mem_free(__p)\
	__map->maf.free(__map->ma_arg, __p)
#define mem_realloc( __p, __n)\
	__map->maf.realloc(__map->ma_arg, __p, __n)
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

ff_err_t static vec_init(vecp __vec, ff_uint_t __blk_size, struct ffly_map_maf *__af, long long *__ma_arg) {
	__vec->bks = __blk_size+sizeof(struct vec_blk);
	__vec->p = NULL;
	__vec->page_c = 0;
	__vec->off = 0;
	__vec->uu = NULL;
	__vec->size = 0;
	__vec->face = NULL;
	__vec->maf = __af;
	__vec->ma_arg = __ma_arg;
	return 0;
}
#define VEC_STUFF
ff_err_t static vec_de_init(vecp __vec) {
	if (__vec->p != NULL) {
		void **pg, **end, *p;
		pg = __vec->p;
		end = pg+__vec->page_c;
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
	mem_free(__vec->p);
	return 0;
}

ff_err_t static vec_push_back(vecp __vec, void **__p) {
	__vec->size++;
	if (!__vec->face) {
		__vec->face = (void**)mem_alloc(__vec->size*sizeof(void*));
	} else {
		__vec->face = (void**)mem_realloc(__vec->face, __vec->size*sizeof(void*));
	}

	void **fp;

	fp = __vec->face+(__vec->size-1);

	if (__vec->uu != NULL) {
		vec_blkp b;
		b = __vec->uu;
		__vec->uu = b->fd;
		b->fd->bk = b->bk;
		b->bk = NULL;
		b->fd = NULL;
		b->n = __vec->size-1;
		b->inuse = 0;
		*fp = *__p = ((ff_u8_t*)b)+sizeof(struct vec_blk);
		return 0;
	}

	ff_uint_t pg, pg_off;
	pg = __vec->off>>PAGE_SHIFT;
	pg_off = __vec->off-(pg<<PAGE_SHIFT);
	if (pg>=__vec->page_c) {
		ff_uint_t pgc;
		pgc = ++__vec->page_c;
		if (!__vec->p) {
			__vec->p = (void**)mem_alloc(pgc*sizeof(void*));
		} else {
			__vec->p = (void**)mem_realloc(__vec->p, pgc*sizeof(void*));
		}
		*(__vec->p+pg) = mem_alloc(PAGE_SIZE*__vec->bks);
	}
	ff_u8_t *p;
	p = ((ff_u8_t*)*(__vec->p+pg))+(pg_off*__vec->bks);
	
	vec_blkp b;
	b = (vec_blkp)p;
	b->bk = NULL;
	b->fd = NULL;
	b->off = __vec->off;
	b->inuse = 0;
	b->n = __vec->size-1;
	*fp = *__p = (void*)(p+sizeof(struct vec_blk));
	__vec->off++;
	return 0;
}

ff_err_t static vec_pop_back(vecp __vec, void *__p) {
	__vec->size--;
	void *fe;

	fe = *(__vec->face+__vec->size);
	if (__vec->size>1) {
		__vec->face = (void**)mem_realloc(__vec->face, __vec->size*sizeof(void*));
	}

	ff_u8_t *p;
	p = (ff_u8_t*)__p;

	vec_blkp b;
	b = (vec_blkp)(p-sizeof(struct vec_blk));
	if (b->off == __vec->off-1) {
		__vec->off--;
		ff_uint_t pg;
		pg = __vec->off>>PAGE_SHIFT;
		if (pg < __vec->page_c-1 && __vec->page_c>1) {
			__vec->page_c--;
			mem_free(*(__vec->p+pg));
			__vec->p = (void**)mem_realloc(__vec->p, __vec->page_c*sizeof(void*));
		}
		return 0;
	}
	
	*(__vec->face+b->n) = fe;
	if (__vec->uu != NULL)
		__vec->uu->bk = &b->fd;
	b->fd = __vec->uu;
	__vec->uu = b;
	b->bk = &__vec->uu;
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


ff_err_t _ffly_map_init(ffly_mapp __map, ff_uint_t __size, void(*__grab)(long long, ff_u8_t)) {
#ifdef FF_MAP_SA
	if (__grab != NULL) {
		__grab((long long)__vec->alloc, FF_VGF_MA);
		__grab((long long)__vec->free, FF_VGF_MF);
		__grab((long long)__vec->realloc, FF_VGF_MR);
		goto _sk;
	}
	__map->maf.alloc = dummy_alloc;
	__map->maf.free	= dummy_free;
	__map->maf.realloc = dummy_realloc;
_sk:
#endif
	__map->size = __size;
	__map->table = mem_alloc((map_mask(__map)+1)*sizeof(struct vec*));
	struct vec **itr = (struct vec**)__map->table;
	while(itr != ((struct vec**)__map->table)+map_mask(__map)+1)
		*(itr++) = NULL;
	__map->begin = NULL;
	__map->end = NULL;
	__map->parent = NULL;
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

void ffly_map_free(ffly_mapp __map) {
	mem_free(__map);
}

void ffly_map_destroy(ffly_mapp __map) {
	ffly_map_de_init(__map);
	mem_free(__map);
}

void const* ffly_map_begin(ffly_mapp __map) {
	return (void const*)__map->begin;
}

void const* ffly_map_beg(ffly_mapp __map) {
	return (void const*)__map->begin;
}

void const* ffly_map_end(ffly_mapp __map) {
	return (void const*)__map->end;
}

void ffly_map_del(ffly_mapp __map, void const *__p) {
	map_entry_t *p = (map_entry_t*)__p;
	mem_free((void*)p->key);
	map_entry_t *end;
	vec_pop_back(p->blk, (void*)&end);
	*p = *end;
	mem_free(end);
}

void ffly_map_fd(ffly_mapp __map, void const **__p) {
	*(void**)__p = ((map_entry_t*)*__p)->next;
}

void ffly_map_bk(ffly_mapp __map, void const **__p) {
	*(void**)__p = ((map_entry_t*)*__p)->prev;
}

void ffly_map_itr(ffly_mapp __map, void const **__p, ff_u8_t __dir) {
	if (__dir == MAP_ITR_FD)
		ffly_map_fd(__map, __p);
	else if (__dir == MAP_ITR_BK)
		ffly_map_bk(__map, __p);	
}

void *const ffly_map_getp(void const *__p) {
	return ((map_entry_t*)__p)->p;
}

void ffly_map_parent(ffly_mapp __map, ffly_mapp __child) {
	__child->parent = __map;
}

map_entry_t static* map_find(ffly_mapp __map, ff_u8_t const *__key, ff_uint_t __bc) {
	ff_u64_t val = ffly_hash(__key, __bc);
	struct vec **blk = ((struct vec**)__map->table)+(val&map_mask(__map));
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

	if (__map->parent != NULL)
		return map_find(__map->parent, __key, __bc);
	ffly_fprintf(ffly_log, "diden't find.\n");
	return NULL;
}

ff_err_t ffly_map_put(ffly_mapp __map, ff_u8_t const *__key, ff_uint_t __bc, void *const __p) {
	ff_u64_t val = ffly_hash(__key, __bc);
	struct vec **blk = ((struct vec**)__map->table)+(val&map_mask(__map));
	if (!*blk) {
		*blk = (struct vec*)__ffly_mem_alloc(sizeof(struct vec));
		if (_err(vec_init(*blk, sizeof(map_entry_t*), &__map->maf, &__map->ma_arg))) {
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

	if (__map->end != NULL) {
		((map_entry_t*)__map->end)->next = (void*)*entry;
		(*entry)->prev = __map->end;
		__map->end = (void*)*entry;
	} else
		__map->end = (void*)*entry;

	if (!__map->begin)
		__map->begin = (void*)*entry;
}

void const* ffly_map_fetch(ffly_mapp __map, ff_u8_t const *__key, ff_uint_t __bc) {
	return (void const*)map_find(__map, __key, __bc);
}

void *const ffly_map_get(ffly_mapp __map, ff_u8_t const *__key, ff_uint_t __bc, ff_err_t *__err) {
	map_entry_t *entry = map_find(__map, __key, __bc);
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

ff_err_t ffly_map_de_init(ffly_mapp __map) {
	struct vec **itr = (struct vec**)__map->table;
	while(itr != ((struct vec**)__map->table)+map_mask(__map)) {
		if (*itr != NULL) free_blk(itr);
		itr++;
	}
	mem_free(__map->table);
	return FFLY_SUCCESS;
}
