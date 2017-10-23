# ifndef __ffly__mem__set__h
# define __ffly__mem__set__h
# include <mdlint.h>
# ifdef __cplusplus
extern "C" {
# endif
void* ffly_mem_set(void*, mdl_u8_t, mdl_uint_t);
# ifdef __cplusplus
}
namespace mdl {
namespace firefly {
namespace data {
static void*(*mem_set)(void*, mdl_u8_t, mdl_uint_t) = &ffly_mem_set;
}
}
}
# endif
# endif /*__ffly__mem__set__h*/
