# ifndef __ffly__config__h
# define __ffly__config__h
# include "system/buff.h"
# include "system/vec.h"
# include "system/map.h"
# include "ffint.h"
# include "types.h"
# define ffly_conf_is_str(__p) \
	(((struct ffly_conf_val*)__p)->kind == _ffly_conf_str)
# define ffly_conf_is_chr(__p) \
	(((struct ffly_conf_val*)__p)->kind == _ffly_conf_chr)
# define ffly_conf_is_64l_u(__p) \
	(((struct ffly_conf_val*)__p)->kind == _ffly_conf_64l_u)
# define ffly_conf_is_32l_u(__p) \
	(((struct ffly_conf_val*)__p)->kind == _ffly_conf_32l_u)
# define ffly_conf_is_16l_u(__p) \
	(((struct ffly_conf_val*)__p)->kind == _ffly_conf_16l_u)
# define ffly_conf_is_8l_u(__p) \
	(((struct ffly_conf_val*)__p)->kind == _ffly_conf_8l_u)
# define ffly_conf_is_64l_s(__p) \
	(((struct ffly_conf_val*)__p)->kind == _ffly_conf_64l_s)
# define ffly_conf_is_32l_s(__p) \
	(((struct ffly_conf_val*)__p)->kind == _ffly_conf_32l_s)
# define ffly_conf_is_16l_s(__p) \
	(((struct ffly_conf_val*)__p)->kind == _ffly_conf_16l_s)
# define ffly_conf_is_8l_s(__p) \
	(((struct ffly_conf_val*)__p)->kind == _ffly_conf_8l_s)

enum {
	_ffly_conf_val,
	_ffly_conf_var,
	_ffly_conf_arr,
	_ffly_conf_struct
};

struct ffly_conf_val {
	ff_u8_t kind;
	ff_byte_t *p;
};

struct ffly_conf_var {
	struct ffly_conf_val *val;
	char *name;
};

struct ffly_conf_arr {
	struct ffly_vec data;
	char *name;
	ff_uint_t l;
};

struct ffly_conf_struct {
	struct ffly_map fields;
	char *name;
};

struct ffly_conf_entity {
	ff_u8_t kind;
	void *p;
};

// config bare
// all in one
typedef struct {
	struct ffly_vec entities;
	struct ffly_vec free;
	struct ffly_map env;
} ffconf;

typedef ffconf* ffconfp;

struct ffly_conf {
	struct ffly_buff sbuf;
	ff_byte_t *p, *end;
	ff_off_t off;
	struct ffly_vec toks;
	struct ffly_buff iject_buff;

	struct ffly_vec entities;
	struct ffly_vec free;
	struct ffly_map env;
};

enum {
	_ffly_conf_str,
	_ffly_conf_chr,
// signed int
	_ffly_conf_64l_s,
	_ffly_conf_32l_s,
	_ffly_conf_16l_s,
	_ffly_conf_8l_s,
// unsigned int
	_ffly_conf_64l_u,
	_ffly_conf_32l_u,
	_ffly_conf_16l_u,
	_ffly_conf_8l_u
};

void const* ffly_conf_get(ffconfp, char const*);
ff_uint_t ffly_conf_arr_len(void const*);
void const* ffly_conf_arr_elem(void const*, ff_uint_t);
ff_err_t ffly_conf_init(struct ffly_conf*);
ff_err_t ffly_conf_read(struct ffly_conf*);
ff_err_t ffly_conf_ld(struct ffly_conf*, char const*);
ff_err_t ffly_conf_free(struct ffly_conf*);
ff_err_t ffconf_free(ffconfp);
void ffly_conf_depos(struct ffly_conf*, ffconfp);
char const* ffly_conf_str(void const*);
char ffly_conf_chr(void const*);
ff_u64_t ffly_conf_64l_u(void const*);
ff_u32_t ffly_conf_32l_u(void const*);
ff_u16_t ffly_conf_16l_u(void const*);
ff_u8_t ffly_conf_8l_u(void const*);
ff_u64_t ffly_conf_int_u(void const*);

ff_i64_t ffly_conf_64l_s(void const*);
ff_i32_t ffly_conf_32l_s(void const*);
ff_i16_t ffly_conf_16l_s(void const*);
ff_i8_t ffly_conf_8l_s(void const*);
ff_i64_t ffly_conf_int_s(void const*);
void const* ffly_conf_struc_get(void const*, char const*);
# endif /*__ffly__config__h*/
