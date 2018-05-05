# ifndef __ffly__colour__h
# define __ffly__colour__h
# include "../types.h"
/*
	// shoud pack structures
*/
typedef struct {
	ff_byte_t r, g, b, a;
} ffly_colour_t;

typedef struct {
    ff_byte_t *r, *g, *b, *a;
} ffly_colourp_t;


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
# endif
# endif /*__ffly__colour__h*/
