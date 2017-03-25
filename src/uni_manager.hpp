# ifndef __uni__manager__hpp
# define __uni__manager__hpp
# include "types/pixmap_t.h"
# include "types/uni_par_t.hpp"
# include <eint_t.hpp>
# include "ffly_config.hpp"
namespace mdl {
namespace firefly {
class uni_manager {
	public:
	uni_manager(uint_t __xaxis_len, uint_t __yaxis_len, uint_t __zaxis_len)
	: xaxis_len(__xaxis_len), yaxis_len(__yaxis_len), zaxis_len(__zaxis_len) {}

	boost::int8_t init(uint_t __xaxis_split, uint_t __yaxis_split, uint_t __zaxis_split);

	private:
	uint_t const xaxis_len, yaxis_len, zaxis_len;
	types::pixmap_t *uni_pixmap = nullptr;
	types::uni_par_t **uni_particles = nullptr;
};
}
}


# endif /*__uni__manager__hpp*/
