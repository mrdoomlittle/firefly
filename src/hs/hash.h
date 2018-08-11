# ifndef __ffly__hs__hash__h
# define __ffly__hs__hash__h
# include "../ffint.h"
struct hash_entry {
	struct hash_entry *fd, *next;
	ff_u8_t const *key;
	ff_uint_t len;
	void *p;
};

struct hash {
	struct hash_entry **table;
	struct hash_entry *top;
};

void hash_init(struct hash*);
void hash_put(struct hash*, ff_u8_t const*, ff_uint_t, void*);
void* hash_get(struct hash*, ff_u8_t const*, ff_uint_t);
# endif /*__ffly__hs__hash__h*/
