# include "as.h"
# include "../system/util/hash.h"
# include "../string.h"
# include "../ffly_def.h"
void ff_as_hash_init(struct hash *__table) {
	__table->table = (hash_entryp*)ff_as_al(0x100*sizeof(struct hash_entry*));
	struct hash_entry **p = __table->table;
	struct hash_entry **bot = p+0x100;
	while(p != bot)
		*(p++) = NULL;
}

struct hash_entry static*
lookup(struct hash *__table, ff_u8_t const *__key, ff_uint_t __len) {
	ff_u64_t sum = ffly_hash(__key, __len);
	struct hash_entry *entry = *(__table->table+(sum&0xff));
	while(entry != NULL) {
		if (entry->len == __len) {
			if (!memcmp(entry->key, __key, __len)) return entry;
		}
		entry = entry->next;
	}
	return NULL;
}

void ff_as_hash_put(struct hash *__table, ff_u8_t const *__key, ff_uint_t __len, void *__p) {
	hash_entryp entry;
	if ((entry = lookup(__table, __key, __len)) != NULL) {
		entry->p = __p;
		return;
	}
	
	ff_u64_t sum = ffly_hash(__key, __len);
	struct hash_entry **table = __table->table+(sum&0xff);

	entry = (hash_entryp)ff_as_al(sizeof(struct hash_entry)); 
	entry->next = *table;
	*table = entry;

	entry->key = ff_as_memdup((void*)__key, __len);
	entry->p = __p;
	entry->len = __len;
}

void* ff_as_hash_get(struct hash *__table, ff_u8_t const *__key, ff_uint_t __len) {
	hash_entryp entry = lookup(__table, __key, __len);
	if (!entry) return NULL;
	return entry->p;
}

