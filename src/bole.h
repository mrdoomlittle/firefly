# ifndef __ffly__boleig__h
# define __ffly__boleig__h
# include "system/buff.h"
# include "system/vec.h"
# include "system/map.h"
# include "ffint.h"
# include "types.h"
# define ffly_bole_is_str(__p) \
	(((struct ffly_bole_val*)__p)->kind == _ffly_bole_str)
# define ffly_bole_is_chr(__p) \
	(((struct ffly_bole_val*)__p)->kind == _ffly_bole_chr)
# define ffly_bole_is_64l_u(__p) \
	(((struct ffly_bole_val*)__p)->kind == _ffly_bole_64l_u)
# define ffly_bole_is_32l_u(__p) \
	(((struct ffly_bole_val*)__p)->kind == _ffly_bole_32l_u)
# define ffly_bole_is_16l_u(__p) \
	(((struct ffly_bole_val*)__p)->kind == _ffly_bole_16l_u)
# define ffly_bole_is_8l_u(__p) \
	(((struct ffly_bole_val*)__p)->kind == _ffly_bole_8l_u)
# define ffly_bole_is_64l_s(__p) \
	(((struct ffly_bole_val*)__p)->kind == _ffly_bole_64l_s)
# define ffly_bole_is_32l_s(__p) \
	(((struct ffly_bole_val*)__p)->kind == _ffly_bole_32l_s)
# define ffly_bole_is_16l_s(__p) \
	(((struct ffly_bole_val*)__p)->kind == _ffly_bole_16l_s)
# define ffly_bole_is_8l_s(__p) \
	(((struct ffly_bole_val*)__p)->kind == _ffly_bole_8l_s)

/*
	TODO:
		cleanup
		replace 'free' vec with linked list struct 

		changed name to bole i dont know if its a fitting name.
*/

enum {
	_ffly_bole_val,
	_ffly_bole_var,
	_ffly_bole_arr,
	_ffly_bole_struct
};

struct ffly_bole_val {
	ff_u8_t kind;
	ff_byte_t *p;
};

struct ffly_bole_var {
	struct ffly_bole_val *val;
	char *name;
};

struct ffly_bole_arr {
	struct ffly_vec data;
	char *name;
	ff_uint_t l;
};

struct ffly_bole_struct {
	struct ffly_map fields;
	char *name;
};

struct ffly_bole_entity {
	ff_u8_t kind;
	void *p;
};

// boleig bare
// all in one
typedef struct {
	struct ffly_vec entities;
	struct ffly_vec free;
	struct ffly_map env;
} ffbole;

typedef ffbole* ffbolep;

struct ffly_bole {
	struct ffly_buff sbuf;
	ff_byte_t *p, *end;
	ff_off_t off;
	struct ffly_vec toks;
	// rename to tokbuf
	struct ffly_buff tokbuf;

	struct ffly_vec entities;
	struct ffly_vec free;
	struct ffly_map env;
};

enum {
	_ffly_bole_str,
	_ffly_bole_chr,
// signed int
	_ffly_bole_64l_s,
	_ffly_bole_32l_s,
	_ffly_bole_16l_s,
	_ffly_bole_8l_s,
// unsigned int
	_ffly_bole_64l_u,
	_ffly_bole_32l_u,
	_ffly_bole_16l_u,
	_ffly_bole_8l_u
};

void const* ffly_bole_get(ffbolep, char const*);
ff_uint_t ffly_bole_arr_len(void const*);
void const* ffly_bole_arr_elem(void const*, ff_uint_t);
ff_err_t ffly_bole_init(struct ffly_bole*);
ff_err_t ffly_bole_read(struct ffly_bole*);
ff_err_t ffly_bole_ld(struct ffly_bole*, char const*);
ff_err_t ffly_bole_free(struct ffly_bole*);
ff_err_t ffbole_free(ffbolep);
void ffly_bole_depos(struct ffly_bole*, ffbolep);
char const* ffly_bole_str(void const*);
char ffly_bole_chr(void const*);
ff_u64_t ffly_bole_64l_u(void const*);
ff_u32_t ffly_bole_32l_u(void const*);
ff_u16_t ffly_bole_16l_u(void const*);
ff_u8_t ffly_bole_8l_u(void const*);
ff_u64_t ffly_bole_int_u(void const*);

ff_i64_t ffly_bole_64l_s(void const*);
ff_i32_t ffly_bole_32l_s(void const*);
ff_i16_t ffly_bole_16l_s(void const*);
ff_i8_t ffly_bole_8l_s(void const*);
ff_i64_t ffly_bole_int_s(void const*);
void const* ffly_bole_struc_get(void const*, char const*);
# endif /*__ffly__boleig__h*/
