# ifndef __player__info__t__hpp
# define __player__info__t__hpp
# include <serializer.hpp>
# include <eint_t.hpp>
namespace mdl {
namespace firefly {
namespace types {

typedef struct player_info {
	uint_t xaxis, yaxis, zaxis;
	void achieve(serializer& __arc) {
		__arc & sizeof(uint_t);
		__arc << xaxis;

		__arc & sizeof(uint_t);
		__arc << yaxis;

		__arc & sizeof(uint_t);
		__arc << zaxis;
	}
} player_info_t;

}
}
}

# endif /*__player__info__t__hpp*/
