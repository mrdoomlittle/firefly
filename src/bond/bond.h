# ifndef __ffly__bond__h
# define __ffly__bond__h
# include <mdlint.h>
# include "../ffef.h"

struct hash_entry {
	struct hash_entry *next;
	mdl_u8_t const *key;
	mdl_uint_t len;
	void *p;
};

typedef struct hash {
	struct hash_entry **table;
} *hashp;
void hash_init(hashp);
void hash_destroy(hashp);
void hash_put(hashp, mdl_u8_t const*, mdl_uint_t, void*);
void* const hash_get(hashp, mdl_u8_t const*, mdl_uint_t);
void bond(char const*, char const*);
# endif /*__ffly__bond__h*/
