# ifndef __ffly__config__h
# define __ffly__config__h
# include "system/buff.h"
# include "types/byte_t.h"
# include "types/off_t.h"
# include "system/vec.h"
# include "system/map.h"
# include <mdlint.h>
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
struct ffly_conf_val {
	mdl_u8_t kind;
	ffly_byte_t *p;
};

struct ffly_conf_var {
	struct ffly_conf_val val;
	char *name;
};

struct ffly_conf_arr {
	struct ffly_vec data;
	char *name;
};

// config bare
typedef struct {
	struct ffly_vec arrs;
	struct ffly_vec free;
	struct ffly_map env;
} ffconf;

typedef ffconf* ffconfp;
struct ffly_conf {
	struct ffly_buff sbuf;
	ffly_byte_t *p, *end;
	ffly_off_t off;
	struct ffly_vec toks;
	struct ffly_buff iject_buff;

	struct ffly_vec arrs;
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
void const* ffly_conf_get_arr_elem(void const*, mdl_uint_t);
ffly_err_t ffly_conf_init(struct ffly_conf*);
ffly_err_t ffly_conf_read(struct ffly_conf*);
ffly_err_t ffly_conf_ld(struct ffly_conf*, char const*);
ffly_err_t ffly_conf_free(struct ffly_conf*);
ffly_err_t ffconf_free(ffconfp);
void ffly_conf_depos(struct ffly_conf*, ffconfp);
char const* ffly_conf_str(void const*);
char ffly_conf_chr(void const*);
mdl_u64_t ffly_conf_64l_u(void const*);
mdl_u32_t ffly_conf_32l_u(void const*);
mdl_u16_t ffly_conf_16l_u(void const*);
mdl_u8_t ffly_conf_8l_u(void const*);
mdl_u64_t ffly_conf_int_u(void const*);

mdl_i64_t ffly_conf_64l_s(void const*);
mdl_i32_t ffly_conf_32l_s(void const*);
mdl_i16_t ffly_conf_16l_s(void const*);
mdl_i8_t ffly_conf_8l_s(void const*);
mdl_i64_t ffly_conf_int_s(void const*);
# endif /*__ffly__config__h*/
