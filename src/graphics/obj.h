# ifndef __ffly__graphics__obj__h
# define __ffly__graphics__obj__h
# include "../types/err_t.h"
# include "../obj.h"
# include "../types/byte_t.h"
# ifdef __cplusplus
namespace mdl {
namespace firefly {
namespace graphics {
types::err_t obj_draw(objp __obj, types::byte_t *__dst, uint_t __xa, uint_t __ya, uint_t __za, uint_t __xal, uint_t __yal, uint_t __zal, uint_t __xmax, uint_t __ymax, uint_t __zmax);
}
}
}
# else
ffly_err_t __ffly_obj_draw(ffly_objp, ffly_byte_t*, mdl_uint_t, mdl_uint_t, mdl_uint_t, mdl_uint_t, mdl_uint_t, mdl_uint_t, mdl_uint_t, mdl_uint_t, mdl_uint_t);
# endif
# endif /*__ffly__graphics__obj__h*/
