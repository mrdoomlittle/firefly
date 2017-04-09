# ifndef __player__t__hpp
# define __player__t__hpp
# include <serializer.hpp>
# include <eint_t.hpp>
namespace mdl {
namespace firefly {
namespace types {

typedef struct {
	uint_t xaxis, yaxis, zaxis;
	void achieve(serializer& __arc) {
		__arc & sizeof(uint_t);
		__arc << xaxis;

		__arc & sizeof(uint_t);
		__arc << yaxis;

		__arc & sizeof(uint_t);
		__arc << zaxis;
	}
} player_t;

}
}
}

# endif /*__player__t__hpp*/
