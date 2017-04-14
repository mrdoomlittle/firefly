# ifndef __byte__t__h
# define __byte__t__h
# ifdef __cplusplus
# include <eint_t.hpp>
namespace mdl {
namespace firefly {
namespace types {
typedef mdl::u8_t byte_t;
}
}
}
# else
# include <eint_t.h>
typedef mdl_u8_t ffly_byte_t;
# endif

# endif /*__byte__t__h*/
