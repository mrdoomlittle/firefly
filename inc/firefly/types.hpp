# ifndef __types__hpp
# define __types__hpp

// needs removing&moving
namespace mdl {
typedef struct {
	int r_xaxis, r_yaxis;
	int s_xaxis, s_yaxis;
	int w_xaxis, w_yaxis;
} mouse_coords_t;

typedef struct {
	int xaxis, yaxis;
} window_coords_t;
}
# endif /*__types__hpp*/
