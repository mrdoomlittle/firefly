# ifndef __ffly__hash__h
# define __ffly__hash__h
# include "../../ffint.h"
# ifdef __cplusplus
extern "C" {
# endif
//ff_u64_t ffly_hash(ff_u8_t const*, ff_uint_t);
extern ff_u64_t(*ffly_hash)(ff_u8_t const*, ff_uint_t);
# ifdef __cplusplus
}
namespace mdl {
namespace firefly {
namespace system {
namespace util {
static ff_u64_t(*hash)(ff_u8_t const*, ff_uint_t) = ffly_hash;
}
}
}
}
# endif
# endif /*__ffly__hash__h*/
