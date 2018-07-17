# include "bond.h"
# include "../string.h"
# include "../malloc.h"
# include "../system/util/hash.h"
# include "../stdio.h"
void
ff_bond_hash_init(hashp __hash) {
	__hash->table = (struct hash_entry**)malloc(0x100*sizeof(struct hash_entry*));
	struct hash_entry **p = __hash->table;
	struct hash_entry **end = p+0x100;
	while(p != end)
		*(p++) = NULL;
}

void static
free_chain(struct hash_entry *__top) {
	struct hash_entry *cur = __top, *bk;
	while(cur != NULL) {
		bk = cur;
		cur = cur->next;
		free((void*)bk->key);
		free(bk);
	}
}

void
ff_bond_hash_destroy(hashp __hash) {
	struct hash_entry **p = __hash->table;
	struct hash_entry **end = p+0x100;
	while(p != end) {
		if (*p != NULL)
			free_chain(*p);
		p++;
	}
	free(__hash->table);
}

void
ff_bond_hash_put(hashp __hash, ff_u8_t const *__key, ff_uint_t __len, void *__p) {
	ff_u64_t sum = ffly_hash(__key, __len);
	struct hash_entry *entry = (struct hash_entry*)malloc(sizeof(struct hash_entry));
	struct hash_entry **table = __hash->table+(sum&0xff);


	entry->next = *table;
	*table = entry;
	printf("hash_put: len: %u\n", __len);
	memdup((void**)&entry->key, (void*)__key, __len);
	entry->p = __p;
	entry->len = __len;
}

void* const
ff_bond_hash_get(hashp __hash, ff_u8_t const *__key, ff_uint_t __len) {
	ff_u64_t sum = ffly_hash(__key, __len);
	struct hash_entry *cur = *(__hash->table+(sum&0xff));
	while(cur != NULL) {
		if (cur->len == __len) {
			if (!memcmp(cur->key, __key, __len))
				return cur->p;
		}
		cur = cur->next;
	}

	printf("hash error, not fatal just cant locate entry nothing to panic about.\n");
	return NULL;
}
