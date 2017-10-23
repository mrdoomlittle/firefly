# ifndef __colour__hpp
# define __colour__hpp
# include "../types/colour_t.h"
# include "../types/byte_t.h"
namespace mdl {
namespace firefly {
namespace graphics {
types::colour_t static __inline__ mk_colour(types::byte_t __r, types::byte_t __g, types::byte_t __b, types::byte_t __a) {
    return (types::colour_t) {r:__r, g:__g, b:__b, a:__a};}
}
}
}
# endif /*__colour__hpp*/
