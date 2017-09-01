# ifndef __swp__h
# define __swp__h
# include <eint_t.h>
# ifdef __cplusplus
extern "C" {
# endif
void ffly_swp(void*, void*, mdl_uint_t);
# ifdef __cplusplus
}

namespace mdl {
namespace firefly {
namespace data {
template<typename _T>
void static __inline__ swp(void *__p1, void *__p2) {
	ffly_swp(__p1, __p2, sizeof(_T));}

static void (*swpn)(void*, void*, mdl::uint_t) = &ffly_swp;
}
}
}
# endif

# endif /*__swp__h*/
