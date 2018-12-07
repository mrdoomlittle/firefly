# include "m.h"
# include "../../memory/mem_alloc.h"
# include "../../memory/mem_free.h"
# include "../../memory/mem_realloc.h"
# include "../../dep/mem_cmp.h"
# include "../../system/util/hash.h"
# include "../../dep/mem_dup.h"
struct hash_entry {
	struct hash_entry *next;
	ff_u8_t const *key;
	ff_uint_t len;
	long long p;
};

struct hash {
	struct hash_entry **table;
};

struct hash static **_h = NULL;
ff_u32_t static off = 0;
void static
hash_init(struct hash *__hash) {
	__hash->table = (struct hash_entry**)__ffly_mem_alloc(0x100*sizeof(struct hash_entry*));
	struct hash_entry **p = __hash->table;
	struct hash_entry **end = p+0x100;
	while(p != end)
		*(p++) = NULL;
}

ff_u32_t mfs_hash_new(void) {
	if (!_h) {
		_h = (struct hash**)__ffly_mem_alloc(sizeof(struct hash*));
		off++;
	} else
		_h = (struct hash**)__ffly_mem_realloc(_h, (++off)*sizeof(struct hash*));
	struct hash *h;
	ff_u32_t r;
	h = (*(_h+(r = (off-1))) = __ffly_mem_alloc(sizeof(struct hash)));
	hash_init(h);
	return r;
}

void
mfs_hash_put(ff_u32_t __h, ff_u8_t const *__key, ff_uint_t __len, long long __p) {
	struct hash *h;

	h = *(_h+__h);
	ff_u64_t sum = ffly_hash(__key, __len);
	struct hash_entry *entry = (struct hash_entry*)__ffly_mem_alloc(sizeof(struct hash_entry));
	struct hash_entry **table = h->table+(sum&0xff);


	entry->next = *table;
	*table = entry;

	ffly_mem_dup((void**)&entry->key, (void*)__key, __len);
	entry->p = __p;
	entry->len = __len;
}

long long
mfs_hash_get(ff_u32_t __h, ff_u8_t const *__key, ff_uint_t __len, ff_i8_t *__found) {
	struct hash *h;

	h = *(_h+__h);
	ff_u64_t sum = ffly_hash(__key, __len);
	struct hash_entry *cur = *(h->table+(sum&0xff));
	while(cur != NULL) {
		if (cur->len == __len) {
			if (!ffly_mem_cmp(cur->key, __key, __len)) {
				*__found = 0;
				return cur->p;
			}
		}
		cur = cur->next;
	}

	*__found = -1;
	return 0;
}

