# ifndef __ffly__bond__h
# define __ffly__bond__h
# include <mdlint.h>
# include "../ffef.h"
/*
	segments will be freed on output
*/
struct hash_entry {
	struct hash_entry *next;
	mdl_u8_t const *key;
	mdl_uint_t len;
	void *p;
};

typedef struct hash {
	struct hash_entry **table;
} *hashp;

typedef struct segment {
	struct segment *next;
	mdl_u8_t *p;
	mdl_uint_t offset;
	mdl_uint_t size;
	mdl_uint_t addr;
	mdl_u8_t type;
} *segmentp;

typedef struct hook* hookp;
typedef struct region {
	struct region *next;
	mdl_u8_t type;
	mdl_u64_t beg, end;
	mdl_uint_t adr;
} *regionp;

typedef struct symbol {
	struct symbol *next;
	char const *name;
	mdl_u8_t type;
	mdl_uint_t loc;
	regionp *reg;
} *symbolp;

typedef struct relocate {
	struct relocate *next;
	mdl_u64_t offset;
	mdl_u8_t l;
	symbolp sy;
} *relocatep;

typedef struct hook {
	struct hook *next;
	mdl_uint_t offset;
	symbolp to;
	mdl_uint_t l;
} *hookp;

struct ffly_bond {


};

regionp extern curbin;
segmentp extern curseg;
mdl_uint_t extern offset;
mdl_uint_t extern adr;
int extern d;

void oust(void*, mdl_uint_t);

void hash_init(hashp);
void hash_destroy(hashp);
void hash_put(hashp, mdl_u8_t const*, mdl_uint_t, void*);
void* const hash_get(hashp, mdl_u8_t const*, mdl_uint_t);
void bond(char const*, char const*);
void output(ffef_hdrp);
void bond_write(mdl_u64_t, void*, mdl_uint_t);
void bond_mapout(mdl_u64_t, mdl_uint_t);
void bond_read(mdl_u64_t, void*, mdl_uint_t);

# endif /*__ffly__bond__h*/
