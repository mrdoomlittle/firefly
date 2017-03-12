# ifndef __obj__info__t__hpp
# define __obj__info__t__hpp
# include <eint_t.hpp>
# include "colour_t.hpp"
# include "atom_t.hpp"
# include "coords_t.hpp"
# include "velocity_t.hpp"
namespace mdl {
namespace firefly {
namespace types {
typedef struct {
	bool *edge, *face;
} collision_t;

typedef struct {
	bool gravity_enabled = false;
	uint_t gravity_speed = 100; // pixels per second
	bool velocity_enabled = false;
	velocity_t velocity;

	bool bound_enabled = false;
	bool bounce_enabled = false;
	//atom_t **atoms;
	uint_t weight, mass = 212;
	uint_t bounce, faces, edges;
	coords_t **edge_coords, **face_coords;

	//bool *collision = nullptr;
	collision_t collision;
	bool *bound_collision;

	uint_t xaxis_bound[2], yaxis_bound[2], zaxis_bound[2];
//	uint_t xaxis, yaxis, zaxis;
	coords_t coords;
	uint_t xaxis_len, yaxis_len, zaxis_len;
	graphics::colour_t def_colour;
	bool skip_handle = false;
} obj_info_t;
}
}
}

# endif /*__obj__info__t__hpp*/
