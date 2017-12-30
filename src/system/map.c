# include "map.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "util/hash.h"
# include "../data/mem_cmp.h"
# include "../data/mem_dupe.h"
# include "errno.h"
# include "err.h"
# include "io.h"
/*still working on this*/
typedef struct {
	mdl_u64_t val;
	mdl_u8_t const *key;
	mdl_uint_t bc;
	void const *p;
    void *prev, *next;
} map_entry_t;

ffly_err_t ffly_map_init(struct ffly_map *__map) {
	__map->table = (struct ffly_vec**)__ffly_mem_alloc(FFLY_MAP_SIZE*sizeof(struct ffly_vec*));
	struct ffly_vec **itr = __map->table;
	while(itr != __map->table+FFLY_MAP_SIZE) *(itr++) = NULL;
    __map->begin = NULL;
    __map->end = NULL;
	return FFLY_SUCCESS;
}

void* ffly_map_begin(struct ffly_map *__map) {
    return __map->begin;
}

void* ffly_map_end(struct ffly_map *__map) {
    return __map->end;
}

void ffly_map_itr(struct ffly_map *__map, void **__itrp, mdl_u8_t __dir) {
    if (__dir == MAP_ITR_FD)
        *__itrp = ((map_entry_t*)*__itrp)->next;
    else if (__dir == MAP_ITR_BK)
        *__itrp = ((map_entry_t*)*__itrp)->prev;    
}

void const* ffly_map_getp(void *__p) {
    return ((map_entry_t*)__p)->p;
}

map_entry_t static* map_find(struct ffly_map *__map, mdl_u8_t const *__key, mdl_uint_t __bc) {
	mdl_u64_t val = ffly_hash(__key, __bc);
	struct ffly_vec **blk = __map->table+(val&FFLY_MAP_SIZE);
	if (!*blk) {
        ffly_fprintf(ffly_log, "map block is null, nothing hear.\n");
        return NULL;
    }

	map_entry_t **itr = (map_entry_t**)ffly_vec_begin(*blk);
    if (ffly_vec_size(*blk)>0) {
	    while(itr <= (map_entry_t**)ffly_vec_end(*blk)) {
            map_entry_t *entry = *itr;
            if (entry->val == val && entry->bc == __bc)
			    if (!ffly_mem_cmp((void*)__key, (void*)entry->key, __bc)) return *itr;
		    itr++;
	    }
    }

    ffly_fprintf(ffly_log, "diden't find.\n");
	return NULL;
}

ffly_err_t ffly_map_put(struct ffly_map *__map, mdl_u8_t const *__key, mdl_uint_t __bc, void const *__p) {
	mdl_u64_t val = ffly_hash(__key, __bc);
	struct ffly_vec **blk = __map->table+(val&FFLY_MAP_SIZE);

	if (!*blk) {
		*blk = (struct ffly_vec*)__ffly_mem_alloc(sizeof(struct ffly_vec));
		ffly_vec_clear_flags(*blk);
		ffly_vec_tog_flag(*blk, VEC_AUTO_RESIZE);
		if (_err(ffly_vec_init(*blk, sizeof(map_entry_t*)))) {
	        ffly_fprintf(ffly_err, "failed to init vec.\n");
		}
	}

	map_entry_t **entry;
	if (_err(ffly_vec_push_back(*blk, (void**)&entry))) {
        ffly_fprintf(ffly_err, "failed to push map entry.\n");
	}

	mdl_u8_t *key;
	if (_err(ffly_mem_dupe((void**)&key, (void*)__key, __bc))) {
        ffly_fprintf(ffly_err, "failed to dupe key.\n");
	}

    *entry = (map_entry_t*)__ffly_mem_alloc(sizeof(map_entry_t));
	**entry = (map_entry_t) {
		.val = val,
		.key = key,
		.bc = __bc,
		.p = __p,
        .next = NULL
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

void const* ffly_map_get(struct ffly_map *__map, mdl_u8_t const *__key, mdl_uint_t __bc) {
	map_entry_t *entry = map_find(__map, __key, __bc);
	if (!entry) {
        ffly_fprintf(ffly_log, "entry was not found.\n");
        return NULL;
    }
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

ffly_err_t ffly_map_de_init(struct ffly_map *__map) {
	struct ffly_vec **itr = __map->table;
	while(itr != __map->table+FFLY_MAP_SIZE) {
		if (*itr != NULL) free_blk(itr);
		itr++;
	}
	__ffly_mem_free(__map->table);
	return FFLY_SUCCESS;
}
