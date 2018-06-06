# include "map.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "util/hash.h"
# include "../dep/mem_cmp.h"
# include "../dep/mem_dup.h"
# include "errno.h"
# include "err.h"
# include "io.h"
# define map_mask(__map) ((~(ff_u64_t)0)>>(64-__map->size))
/*still working on this*/
typedef struct {
	ff_u64_t val;
	ff_u8_t const *key;
	ff_uint_t bc;
	void *p;
	void *prev, *next;
	struct ffly_vec *blk;
} map_entry_t;

ff_err_t ffly_map_init(ffly_mapp __map, ff_uint_t __size) {
	__map->size = __size;
	__map->table = (struct ffly_vec**)__ffly_mem_alloc((map_mask(__map)+1)*sizeof(struct ffly_vec*));
	struct ffly_vec **itr = __map->table;
	while(itr != __map->table+map_mask(__map)+1)
		*(itr++) = NULL;
	__map->begin = NULL;
	__map->end = NULL;
	__map->parent = NULL;
	return FFLY_SUCCESS;
}

ffly_mapp ffly_map_creat(ff_uint_t __size) {
	ffly_mapp p = (ffly_mapp)__ffly_mem_alloc(sizeof(struct ffly_map));
	ffly_map_init(p, __size);
	return p;
}

ffly_mapp ffly_map(ff_uint_t __size) {
	ffly_mapp p = (ffly_mapp)__ffly_mem_alloc(sizeof(struct ffly_map));
	ffly_map_init(p, __size);
	return p;
}

void ffly_map_free(ffly_mapp __map) {
	__ffly_mem_free(__map);
}

void ffly_map_destroy(ffly_mapp __map) {
	ffly_map_de_init(__map);
	__ffly_mem_free(__map);
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
	__ffly_mem_free((void*)p->key);
	map_entry_t *end;
	ffly_vec_pop_back(p->blk, (void*)&end);
	*p = *end;
	__ffly_mem_free(end);
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
	struct ffly_vec **blk = __map->table+(val&map_mask(__map));
	if (!*blk) {
		ffly_fprintf(ffly_log, "map block is null, nothing hear.\n");
		return NULL;
	}

	map_entry_t **itr;
	if (ffly_vec_size(*blk)>0) {
		itr = (map_entry_t**)ffly_vec_begin(*blk);
		while(itr <= (map_entry_t**)ffly_vec_end(*blk)) {
			map_entry_t *entry = *itr;
			if (entry->val == val && entry->bc == __bc)
				if (!ffly_mem_cmp((void*)__key, (void*)entry->key, __bc)) return *itr;
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
	struct ffly_vec **blk = __map->table+(val&map_mask(__map));
	if (!*blk) {
		*blk = (struct ffly_vec*)__ffly_mem_alloc(sizeof(struct ffly_vec));
		ffly_vec_clear_flags(*blk);
		ffly_vec_set_flags(*blk, VEC_AUTO_RESIZE);
		if (_err(ffly_vec_init(*blk, sizeof(map_entry_t*)))) {
			ffly_fprintf(ffly_err, "failed to init vec.\n");
		}
	}

	map_entry_t **entry;
	if (_err(ffly_vec_push_back(*blk, (void**)&entry))) {
		ffly_fprintf(ffly_err, "failed to push map entry.\n");
	}

	ff_u8_t *key;
	if (_err(ffly_mem_dupe((void**)&key, (void*)__key, __bc))) {
		ffly_fprintf(ffly_err, "failed to dupe key.\n");
	}

	*entry = (map_entry_t*)__ffly_mem_alloc(sizeof(map_entry_t));
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

void static free_blk(struct ffly_vec **__blk) {
	map_entry_t **itr;
	if (ffly_vec_size(*__blk)>0) {
		itr = (map_entry_t**)ffly_vec_begin(*__blk);
		while(itr <= (map_entry_t**)ffly_vec_end(*__blk)) {
			__ffly_mem_free((void*)(*itr)->key);
			__ffly_mem_free(*itr);
			itr++;
		}
	}
	ffly_vec_de_init(*__blk);
	__ffly_mem_free(*__blk);
}

ff_err_t ffly_map_de_init(ffly_mapp __map) {
	struct ffly_vec **itr = __map->table;
	while(itr != __map->table+map_mask(__map)) {
		if (*itr != NULL) free_blk(itr);
		itr++;
	}
	__ffly_mem_free(__map->table);
	return FFLY_SUCCESS;
}
