# ifndef __ffly__clay__hash__h
# define __ffly__clay__hash__h
# include "../ffint.h"
struct clay_hash_entry {
	struct clay_hash_entry *fd, *next;
	ff_u8_t const *key;
	ff_uint_t len;
	void *p;
};

struct clay_hash {
	struct clay_hash_entry **table;
	struct clay_hash_entry *top;
};

void clay_hash_init(struct clay_hash*);
void clay_hash_put(struct clay_hash*, ff_u8_t const*, ff_uint_t, void*);
void* clay_hash_get(struct clay_hash*, ff_u8_t const*, ff_uint_t);
# endif /*__ffly__clay__hash__h*/
