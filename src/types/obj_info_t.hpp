# ifndef __obj__info__t__hpp
# define __obj__info__t__hpp
# include <mdlint.h>
# include "colour_t.hpp"
# include "atom_t.hpp"
# include "coords_t.hpp"
# include "velocity_t.hpp"
# include <atomic>
namespace mdl {
namespace firefly {
namespace types {
typedef struct {
	bool *edge, *face, *vertice;
} collision_t;

struct obj_info_t {
	bool gravity_enabled = false;
	uint_t gravity_speed = 5; // pixels per second
	bool velocity_enabled = false;
	velocity_t velocity;

	bool bound_enabled = false;
	bool bounce_enabled = false;
	//atom_t **atoms;
	uint_t weight, mass = 212;
	uint_t bounce, edges, faces, vertices;
	coords_t<> **edge_coords, **face_coords, **vertice_coords;

	//bool *collision = nullptr;
	bool collision_enabled = false;
	collision_t collision;
	bool *bound_collision;

	uint_t xaxis_bound[2], yaxis_bound[2], zaxis_bound[2];
//	uint_t xaxis, yaxis, zaxis;
	coords_t<std::atomic<uint_t>> coords;
	uint_t xaxis_len, yaxis_len, zaxis_len;
	graphics::colour_t def_colour;
	bool skip_handle = false;
	boost::uint16_t angle = 0;
};
}
}
}

# endif /*__obj__info__t__hpp*/
