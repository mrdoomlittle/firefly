# ifndef __ffly__hash__h
# define __ffly__hash__h
# include <mdlint.h>
# ifdef __cplusplus
extern "C" {
# endif
mdl_u64_t ffly_hash(mdl_u8_t const*, mdl_uint_t);
# ifdef __cplusplus
}
namespace mdl {
namespace firefly {
namespace system {
namespace util {
static mdl_u64_t(*hash)(mdl_u8_t const*, mdl_uint_t) = &ffly_hash;
}
}
}
}
# endif
# endif /*__ffly__hash__h*/
