# include "as.h"
# include "../system/util/hash.h"
# include "../data/mem_cmp.h"
# include "../ffly_def.h"
void hash_init(struct hash *__table) {
	__table->table = (hash_entryp*)_alloca(0x100*sizeof(struct hash_entry*));
	struct hash_entry **p = __table->table;
	struct hash_entry **bot = p+0x100;
	while(p != bot) *(p++) = NULL;
}

struct hash_entry static* find(struct hash *__table, mdl_u8_t const *__key, mdl_uint_t __len) {
	mdl_u64_t sum = ffly_hash(__key, __len);
	struct hash_entry *entry = *(__table->table+(sum&0xff));
	while(entry != NULL) {
		if (entry->len == __len) {
			if (!ffly_mem_cmp(entry->key, __key, __len)) return entry;
		}
		entry = entry->next;
	}
	return NULL;
}

void hash_put(struct hash *__table, mdl_u8_t const *__key, mdl_uint_t __len, void *__p) {
	mdl_u64_t sum = ffly_hash(__key, __len);
	struct hash_entry **table = __table->table+(sum&0xff);
	hash_entryp entry = (hash_entryp)_alloca(sizeof(struct hash_entry)); 
	entry->next = *table;
	*table = entry;

	entry->key = memdup((void*)__key, __len);
	entry->p = __p;
	entry->len = __len;
}

void* hash_get(struct hash *__table, mdl_u8_t const *__key, mdl_uint_t __len) {
	hash_entryp entry = find(__table, __key, __len);
	if (!entry) return NULL;
	return entry->p;
}

