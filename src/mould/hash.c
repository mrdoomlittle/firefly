# include "mould.h"
# include "../string.h"
# include "../malloc.h"
# include "../system/util/hash.h"
# include "../stdio.h"
void hash_init(struct hash *__hash) {
	__hash->table = (struct hash_entry**)malloc(0x100*sizeof(struct hash_entry*));
	struct hash_entry **p = __hash->table;
	struct hash_entry **end = p+0x100;
	while(p != end)
		*(p++) = NULL;
	__hash->top = NULL;
}

void hash_destroy(struct hash *__hash) {
	struct hash_entry *cur = __hash->top, *bk;
	while(cur != NULL) {
		bk = cur;
		cur = cur->fd;
		free(bk->key);
		free(bk);
	}
	free(__hash->table);
}

void hash_put(struct hash *__hash, ff_u8_t const *__key, ff_uint_t __len, void *__p) {
	ff_u64_t sum = ffly_hash(__key, __len);
	struct hash_entry *entry = (struct hash_entry*)malloc(sizeof(struct hash_entry));
	struct hash_entry **table = __hash->table+(sum&0xff);

	entry->next = *table;
	*table = entry;
	memdup((void**)&entry->key, (void*)__key, __len);
	entry->p = __p;
	entry->len = __len;
	entry->fd = __hash->top;
	__hash->top = entry;
}

void* hash_get(struct hash *__hash, ff_u8_t const *__key, ff_uint_t __len) {
	ff_u64_t sum = ffly_hash(__key, __len);
	struct hash_entry *cur = *(__hash->table+(sum&0xff));
	while(cur != NULL) {
		if (cur->len == __len) {
			if (!memcmp(cur->key, __key, __len))
				return cur->p;
		}
		cur = cur->next;
	}
	return NULL;
}
