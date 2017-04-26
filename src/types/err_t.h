# ifndef __err__t__h
# define __err__t__h

# ifdef __cplusplus
# include <eint_t.hpp>
namespace mdl {
namespace firefly {
namespace types {
typedef i8_t err_t;
}
}
}
typedef mdl::i8_t ffly_err_t;
# else
# include <eint_t.h>
typedef mdl_i8_t ffly_err_t;
# endif

# endif /*__err__t__h*/
