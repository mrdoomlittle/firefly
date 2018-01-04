# ifndef __ffly__obj__h
# define __ffly__obj__h
# include "types/err_t.h" 
# include "graphics/image.h"
# include "types/pixelmap_t.h"
# include "system/vec.h"
# define ffly_set_objx(__obj, __x) \
    __obj->xa = __x;    
# define ffly_set_objy(__obj, __y) \
    __obj->ya = __y;
# define ffly_set_objz(__obj, __z) \
    __obj->za = __z;
struct ffly_obj {
    ffly_pixelmap_t texture;
    mdl_uint_t xal, yal, zal;
    mdl_uint_t xa, ya, za;

    /* cleanup
    */
    struct ffly_obj *prev, *next;
};

typedef struct ffly_obj* ffly_objp;
typedef ffly_objp* ffly_objpp;
typedef ffly_objpp* ffly_objppp;
ffly_err_t ffly_obj_draw(ffly_objp, ffly_byte_t*, mdl_uint_t, mdl_uint_t, mdl_uint_t, mdl_uint_t, mdl_uint_t, mdl_uint_t, mdl_uint_t, mdl_uint_t, mdl_uint_t);
ffly_objp ffly_obj_alloc(ffly_err_t*);
ffly_err_t ffly_obj_free(ffly_objp);
ffly_err_t ffly_obj_handle(ffly_objp);
ffly_err_t ffly_obj_cleanup();
# endif /*__ffly__obj__h*/
