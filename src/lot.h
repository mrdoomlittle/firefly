# ifndef __ffly__lot__h
# define __ffly__lot__h
# include "obj.h"
# include <mdlint.h> 
enum {
    _ffly_lotsize_8 = 3,
    _ffly_lotsize_16,
    _ffly_lotsize_32,
    _ffly_lotsize_64,
    _ffly_lotsize_128
};

struct ffly_lot {
    ffly_objpp top;
    ffly_objpp end;
    ffly_objppp objs;
    mdl_uint_t xl, yl, zl;
    mdl_uint_t x, y, z;
    /*
        cleanup
    */
    struct ffly_lot *prev, *next;
};

typedef struct ffly_lot* ffly_lotp;
typedef ffly_lotp* ffly_lotpp;
# ifdef __cplusplus
extern "C" {
# endif
void ffly_lot_add(ffly_lotp, ffly_objp);
void ffly_lot_rm(ffly_lotp, ffly_objp);
ffly_objpp ffly_lot_obj(ffly_lotp, ffly_objp);
void ffly_lot_prepare(ffly_lotp, mdl_uint_t, mdl_uint_t, mdl_uint_t);
ffly_lotp ffly_alloc_lot(mdl_uint_t, mdl_uint_t, mdl_uint_t);
void ffly_lot_cleanup();
void ffly_free_lot(ffly_lotp);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__lot__h*/
