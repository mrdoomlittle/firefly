# include "hash.h"
# include "../system/util/hash.h"
# include "../dep/mem_cpy.h"
# include "../dep/mem_cmp.h"
# include "../ffly_def.h"
# include "memalloc.h"

void static*
memdup(void *__src, ff_uint_t __n) {
	void *r;
	ffly_mem_cpy(r = ff_hs_mem_alloc(__n), __src, __n);
	return r;
}

void hs_hash_init(struct hs_hash *__hash) {
	__hash->table = (struct hs_hash_entry**)ff_hs_mem_alloc(0x100*sizeof(struct hs_hash_entry*));
	struct hs_hash_entry **p = __hash->table;
	struct hs_hash_entry **end = p+0x100;
	while(p != end)
		*(p++) = NULL;
	__hash->top = NULL;
}

void hs_hash_put(struct hs_hash *__hash, ff_u8_t const *__key, ff_uint_t __len, void *__p) {
	ff_u64_t sum = ffly_hash(__key, __len);
	struct hs_hash_entry *entry = (struct hs_hash_entry*)ff_hs_mem_alloc(sizeof(struct hs_hash_entry));
	struct hs_hash_entry **table = __hash->table+(sum&0xff);

	entry->next = *table;
	*table = entry;
	entry->key = memdup((void*)__key, __len);
	entry->p = __p;
	entry->len = __len;
	entry->fd = __hash->top;
	__hash->top = entry;
}

void* hs_hash_get(struct hs_hash *__hash, ff_u8_t const *__key, ff_uint_t __len) {
	ff_u64_t sum = ffly_hash(__key, __len);
	struct hs_hash_entry *cur = *(__hash->table+(sum&0xff));
	while(cur != NULL) {
		if (cur->len == __len) {
			if (!ffly_mem_cmp(cur->key, __key, __len))
				return cur->p;
		}
		cur = cur->next;
	}
	return NULL;
}
