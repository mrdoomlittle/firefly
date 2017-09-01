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
//# ifdef LAYER_MANAGER
	uint_t bse_layer_id;
	bool add_bse_layer;
//# endif
# ifdef __WITH_OBJ_MANAGER
	void *obj_manger_ptr;
# endif
# ifdef ROOM_MANAGER
	bool add_bse_room;
	bool change_room;
# endif
# ifdef __WITH_UNI_MANAGER
	bool uni_init;
# endif
} init_opt_t;
# elif defined(FFLY_STUDIO)
typedef struct {

} init_opt_t;
# else
#	error ""
# endif
}
}
}

# endif /*__init__opt__hpp*/
