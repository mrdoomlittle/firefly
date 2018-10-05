# ifndef __ffly__hs__hash__h
# define __ffly__hs__hash__h
# include "../ffint.h"
struct hs_hash_entry {
	struct hs_hash_entry *fd, *next;
	ff_u8_t const *key;
	ff_uint_t len;
	void *p;
};

struct hs_hash {
	struct hs_hash_entry **table;
	struct hs_hash_entry *top;
};

void hs_hash_init(struct hs_hash*);
void hs_hash_put(struct hs_hash*, ff_u8_t const*, ff_uint_t, void*);
void* hs_hash_get(struct hs_hash*, ff_u8_t const*, ff_uint_t);
# endif /*__ffly__hs__hash__h*/
