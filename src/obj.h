# ifndef __ffly__obj__h
# define __ffly__obj__h
# include "types/err_t.h" 
# include "graphics/image.h"
# include "types/pixelmap_t.h"
# include "system/vec.h"
# include "script.h"
# include "polygon.h"
# define ffly_set_objx(__obj, __x) \
    __obj->x = __x;    
# define ffly_set_objy(__obj, __y) \
    __obj->y = __y;
# define ffly_set_objz(__obj, __z) \
    __obj->z = __z;
# define ffly_obj_vertex(__obj, __x, __y, __z) \
    ffly_vertex3(&(__obj)->shape, __x, __y, __z) 
struct ffly_obj {
    ffly_polygon shape;
    ffly_pixelmap_t texture;
    mdl_uint_t xl, yl, zl;
    mdl_uint_t x, y, z;
    ffscriptp script;
    /* cleanup
    */
    struct ffly_obj *prev, *next;
};

typedef struct ffly_obj* ffly_objp;
typedef ffly_objp* ffly_objpp;
typedef ffly_objpp* ffly_objppp;

# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_obj_draw(ffly_objp, ffly_byte_t*, mdl_uint_t, mdl_uint_t, mdl_uint_t, mdl_uint_t, mdl_uint_t, mdl_uint_t);
ffly_objp ffly_obj_alloc(ffly_err_t*);
ffly_err_t ffly_obj_free(ffly_objp);
ffly_err_t ffly_obj_handle(ffly_objp);
ffly_err_t ffly_obj_cleanup();
# ifdef __cplusplus
}
namespace mdl {
namespace firefly {
typedef ffly_objp objp;
typedef ffly_objpp objpp;
typedef ffly_objppp objppp;
struct obj {

};
}
}
# endif
# endif /*__ffly__obj__h*/
