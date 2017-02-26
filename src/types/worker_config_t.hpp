# ifndef __worker__config__t__hpp
# define __worker__config__t__hpp
# include <serializer.hpp>
# include <eint_t.hpp>
namespace mdl {
namespace firefly {
namespace types {
typedef struct {
	uint_t chunk_xaxis, chunk_yaxis, chunk_zaxis;
	uint_t chunk_xlen, chunk_ylen, chunk_zlen;
	void achieve(serializer& __arc) {
		__arc & sizeof(uint_t);
		__arc << chunk_xaxis;

		__arc & sizeof(uint_t);
		__arc << chunk_yaxis;

		__arc & sizeof(uint_t);
		__arc << chunk_zaxis;

		__arc & sizeof(uint_t);
		__arc << chunk_xlen;

		__arc & sizeof(uint_t);
		__arc << chunk_ylen;

		__arc & sizeof(uint_t);
		__arc << chunk_zlen;
	}
} worker_config_t;

}
}
}

# endif /*__worker__config__t__hpp*/
