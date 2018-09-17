# include "hash.h"
# include "../system/util/hash.h"
# include "../string.h"
# include "../ffly_def.h"
# include "memalloc.h"
void static*
_memdup(void *__src, ff_uint_t __n) {
	void *r;
	memcpy(r = clay_mem_alloc(__n), __src, __n);
	return r;
}

void clay_hash_init(struct clay_hash *__hash) {
	__hash->table = (struct clay_hash_entry**)clay_mem_alloc(0x100*sizeof(struct clay_hash_entry*));
	struct clay_hash_entry **p = __hash->table;
	struct clay_hash_entry **end = p+0x100;
	while(p != end)
		*(p++) = NULL;
	__hash->top = NULL;
}

void clay_hash_put(struct clay_hash *__hash, ff_u8_t const *__key, ff_uint_t __len, void *__p) {
	ff_u64_t sum = ffly_hash(__key, __len);
	struct clay_hash_entry *entry = (struct clay_hash_entry*)clay_mem_alloc(sizeof(struct clay_hash_entry));
	struct clay_hash_entry **table = __hash->table+(sum&0xff);

	entry->next = *table;
	*table = entry;
	entry->key = _memdup((void*)__key, __len);
	entry->p = __p;
	entry->len = __len;
	entry->fd = __hash->top;
	__hash->top = entry;
}

void* clay_hash_get(struct clay_hash *__hash, ff_u8_t const *__key, ff_uint_t __len) {
	ff_u64_t sum = ffly_hash(__key, __len);
	struct clay_hash_entry *cur = *(__hash->table+(sum&0xff));
	while(cur != NULL) {
		if (cur->len == __len) {
			if (!memcmp(cur->key, __key, __len))
				return cur->p;
		}
		cur = cur->next;
	}
	return NULL;
}
