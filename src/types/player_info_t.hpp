# ifndef __player__info__t__hpp
# define __player__info__t__hpp
# include <serializer.hpp>
# include <eint_t.hpp>
namespace mdl {
namespace firefly {
namespace types {

typedef struct player_info {
	int key_code = 0;
	uint_t xaxis, yaxis, zaxis;
	void achieve(serializer& __arc) {
		__arc & sizeof(int);
		__arc << key_code;
	}
} player_info_t;

}
}
}

# endif /*__player__info__t__hpp*/
