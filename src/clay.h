# ifndef __ffly__clay__h
# define __ffly__clay__h
# include "ffint.h"
# include "clay/hash.h"

# define clay_8(__p) \
	(*(ff_u8_t*)ffly_clay_gettext(__p))
# define clay_16(__p) \
	(*(ff_u16_t*)ffly_clay_gettext(__p))
# define clay_32(__p) \
	(*(ff_u32_t*)ffly_clay_gettext(__p))
# define clay_64(__p) \
	(*(ff_u64_t*)ffly_clay_gettext(__p))
typedef struct ffly_clay {
	ff_u8_t *p;
	ff_u32_t off, end;
	struct clay_hash table;
} *ffly_clayp;

struct clay_val {
	ff_u8_t type;
	void *text;
};

struct clay_trove {
	struct clay_hash table;
};

enum {
	_clay_str,
	_clay_8,
	_clay_16,
	_clay_32,
	_clay_64
};

enum {
	_clay_val,
	_clay_trove
};

enum {
	_clay_tok_str,
	_clay_tok_keywd,
	_clay_tok_no,
	_clay_tok_ident
};

enum {
	_clay_l_brace,
	_clay_r_brace,
	_clay_tilde
};

void ffly_clay_init(ffly_clayp);
void ffly_clay_de_init(ffly_clayp);
ff_u8_t clay_nexttokis(ffly_clayp, ff_u8_t, ff_u8_t);
ff_u8_t clay_reckontok(ffly_clayp, ff_u8_t, ff_u8_t);
void ffly_clay_load(ffly_clayp, char const*);
void ffly_clay_read(ffly_clayp);
void* ffly_clay_get(char const*, ffly_clayp);
void* ffly_clay_tget(char const*, void*);
void *ffly_clay_gettext(void*);
# endif /*__ffly__clay__h*/
