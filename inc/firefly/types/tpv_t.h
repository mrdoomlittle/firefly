# ifndef __tpv__t__h
# define __tpv__t__h
# ifdef __cplusplus
# include <eint_t.hpp>
namespace mdl {
namespace firefly {
namespace types {
typedef u64_t tpv_t;
}
}
}
# else
# include <eint_t.h>
typedef mdl_u64_t ffly_tpv_t;
# endif
# endif /*__tpv__t__h*/
