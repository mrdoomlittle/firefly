# ifndef __init__opt__hpp
# define __init__opt__hpp
# include <eint_t.hpp>
/* NOTE: all things like window size and camera size so x and y len
need to be divisabal by 2 all the way to 0 or 1 ??
*/
namespace mdl {
namespace firefly {
namespace types {
# if defined(FFLY_SERVER)

typedef struct {


} init_opt_t;

# elif defined(FFLY_CLIENT)

typedef struct {
	uint_t cam_xlen, cam_ylen;
} init_opt_t;
}
}
}
# else
#	error ""
# endif

# endif /*__init__opt__hpp*/
