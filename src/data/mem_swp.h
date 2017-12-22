# ifndef __ffly__mem__swp__h
# define __ffly__mem__swp__h
# include <mdlint.h>
# ifdef __cplusplus
extern "C" {
# endif
void ffly_mem_swp(void*, void*, mdl_uint_t);
# ifdef __cplusplus
}
namespace mdl {
namespace firefly {
namespace data {
template<typename _T>
void static __inline__ mem_swp(void*__p1, void*__p2) {
	ffly_mem_swp(__p1, __p2, sizeof(_T));}
static void(*mem_swpn)(void*, void*, mdl::uint_t) = &ffly_mem_swp;
}
}
}
# endif
# endif /*__ffly__mem__swp__h*/
