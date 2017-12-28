# ifndef __ffly__config__h
# define __ffly__config__h
# include "system/buff.h"
# include "types/byte_t.h"
# include "types/off_t.h"
# include "system/vec.h"
# include "system/map.h"
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
    _ffly_conf_64l_int,
    _ffly_conf_32l_int,
    _ffly_conf_16l_int,
    _ffly_conf_8l_int
};

ffly_err_t ffly_conf_init(struct ffly_conf*);
ffly_err_t ffly_conf_read(struct ffly_conf*);
ffly_err_t ffly_conf_ld(struct ffly_conf*, char*);
ffly_err_t ffly_conf_free(struct ffly_conf*);
# endif /*__ffly__config__h*/
