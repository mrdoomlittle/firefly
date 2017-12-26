# include "map.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "util/hash.h"
# include "../data/mem_cmp.h"
# include "../data/mem_dupe.h"
# include "errno.h"
# include "err.h"
/*still working on this*/
typedef struct {
	mdl_u64_t val;
	mdl_u8_t const *key;
	mdl_uint_t bc;
	void const *p;
} map_entry_t;

ffly_err_t ffly_map_init(struct ffly_map *__map) {
	__map->table = (struct ffly_vec**)__ffly_mem_alloc(FFLY_MAP_SIZE*sizeof(struct ffly_vec*));
	struct ffly_vec **itr = __map->table;
	while(itr != __map->table+FFLY_MAP_SIZE) *(itr++) = NULL;
}

map_entry_t static* map_find(struct ffly_map *__map, mdl_u8_t const *__key, mdl_uint_t __bc) {
	mdl_u64_t val = ffly_hash(__key, __bc);
	struct ffly_vec **map_blk = __map->table+(val&FFLY_MAP_SIZE);
	if (!*map_blk) return NULL;

	map_entry_t *itr = (map_entry_t*)ffly_vec_begin(*map_blk);
	while(itr <= (map_entry_t*)ffly_vec_end(*map_blk)) {
		if (itr->val == val && itr->bc == __bc)
			if (!ffly_mem_cmp((void*)__key, (void*)itr->key, __bc)) return itr;
		itr++;
	}
	return NULL;
}

ffly_err_t ffly_map_put(struct ffly_map *__map, mdl_u8_t const *__key, mdl_uint_t __bc, void const *__p) {
	mdl_u64_t val = ffly_hash(__key, __bc);
	struct ffly_vec **map_blk = __map->table+(val&FFLY_MAP_SIZE);

	if (!*map_blk) {
		*map_blk = (struct ffly_vec*)__ffly_mem_alloc(sizeof(struct ffly_vec));
		ffly_vec_clear_flags(*map_blk);
		ffly_vec_tog_flag(*map_blk, VEC_AUTO_RESIZE);
		if (_err(ffly_vec_init(*map_blk, sizeof(map_entry_t)))) {
			//err
		}
	}

	map_entry_t *entry;
	if (_err(ffly_vec_push_back(*map_blk, (void**)&entry))) {
		//err
	}
	mdl_u8_t *key;
	if (_err(ffly_mem_dupe((void**)&key, (void*)__key, __bc))) {
		//err
	}

	*entry = (map_entry_t) {
		.val = val,
		.key = key,
		.bc = __bc,
		.p = __p
	};
}

void const* ffly_map_get(struct ffly_map *__map, mdl_u8_t const *__key, mdl_uint_t __bc) {
	map_entry_t *entry = map_find(__map, __key, __bc);
	if (!entry) return NULL;
	return entry->p;
}

void static free_map_blk(struct ffly_vec **__map_blk) {
	map_entry_t *itr = (map_entry_t*)ffly_vec_begin(*__map_blk);
	while(itr <= (map_entry_t*)ffly_vec_end(*__map_blk)) __ffly_mem_free((void*)(itr++)->key);
	ffly_vec_de_init(*__map_blk);
	__ffly_mem_free(*__map_blk);
}

ffly_err_t ffly_map_de_init(struct ffly_map *__map) {
	struct ffly_vec **itr = __map->table;
	while(itr != __map->table+FFLY_MAP_SIZE) {
		if (*itr != NULL) free_map_blk(itr);
		itr++;
	}
	__ffly_mem_free(__map->table);
	return FFLY_SUCCESS;
}
