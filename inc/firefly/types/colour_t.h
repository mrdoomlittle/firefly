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

// replace __colour__ with mk_colour(make colour)
colour_t static __inline__ mk_colour(types::byte_t __r, types::byte_t __g, types::byte_t __b, types::byte_t __a) {
    return (colour_t) {r:__r, g:__g, b:__b, a:__a};}
colour_t static __inline__ __colour__(types::byte_t __r, types::byte_t __g, types::byte_t __b, types::byte_t __a) {
    return (colour_t) {r:__r, g:__g, b:__b, a:__a};}
}
}
}
# endif
# endif /*__colour__t__h*/
