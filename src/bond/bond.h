# ifndef __ffly__bond__h
# define __ffly__bond__h
# include "../ffint.h"
# include "../remf.h"
# include "frag.h"
#define imos(__off)\
	offset = __off
/*
	segments will be freed on output
*/
struct hash_entry {
	struct hash_entry *next;
	ff_u8_t const *key;
	ff_uint_t len;
	void *p;
};

typedef struct hash {
	struct hash_entry **table;
} *hashp;

typedef struct segment {
	struct segment *next;
	ff_u8_t *p;
	ff_uint_t offset;
	ff_uint_t size;
	ff_uint_t addr;
	ff_u8_t type;
} *segmentp;

typedef struct hook* hookp;

/*
put in own region 'fragmentitized region'
*/
typedef struct region {
	struct region *next;
	ff_u8_t type;
	ff_u64_t offset;
	ff_u32_t size;
	ff_uint_t adr;
	ff_u16_t fs, nf;
} *regionp;

typedef struct symbol {
	struct symbol *next;
	char const *name;
	ff_u8_t type;
	ff_uint_t loc;
	regionp *reg;
	ff_u16_t f;
} *symbolp;

typedef struct relocate {
	struct relocate *next;
	ff_u64_t offset;
	ff_u16_t adr;
	// fragment of symbol/label
	ff_u16_t to;
	// fragment reloc is in
	ff_u16_t f;
	ff_u8_t flags;
} *relocatep;

typedef struct hook {
	struct hook *next;
	ff_uint_t offset;
	symbolp to;
	ff_u16_t adr;
	ff_uint_t f;
	ff_u8_t flags;
} *hookp;

struct ffly_bond {


};

regionp extern curbin;
segmentp extern curseg;
ff_uint_t extern offset;
ff_uint_t extern adr;
int extern d;

void bond_oust(void*, ff_uint_t);

void bond_hash_init(hashp);
void bond_hash_destroy(hashp);
void bond_hash_put(hashp, ff_u8_t const*, ff_uint_t, void*);
void* const bond_hash_get(hashp, ff_u8_t const*, ff_uint_t);
void bond(char const*, char const*);
void bond_output(remf_hdrp);
void bond_write(ff_u64_t, void*, ff_uint_t);
void bond_mapout(ff_u64_t, ff_uint_t);
void bond_read(ff_u64_t, void*, ff_uint_t);
# endif /*__ffly__bond__h*/
