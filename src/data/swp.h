# ifndef __swp__h
# define __swp__h
# include <eint_t.h>
# ifdef __cplusplus
extern "C" {
# endif
void ffly_swp(void*, void*, mdl_uint_t);
# ifdef __cplusplus
}
# endif

# ifdef __cplusplus
namespace mdl {
namespace firefly {
namespace data {
static void (*swp)(void*, void*, mdl::uint_t) = &ffly_swp;
}
}
}
# endif

# endif /*__swp__h*/
