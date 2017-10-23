# ifndef __colour__t__h
# define __colour__t__h
# include "byte_t.h"
typedef struct {
	ffly_byte_t r, g, b, a;
} ffly_colour_t;

# ifdef __cplusplus
namespace mdl {
namespace firefly {
namespace types {
typedef ffly_colour_t colour_t;

typedef struct {
    types::byte_t r, g, b, a;
} rgba_colour_t;

typedef struct {
    types::byte_t r, g, b;
} rgb_colour_t;
}
}
}
# endif // __cplusplus
# endif /*__colour__t__h*/
