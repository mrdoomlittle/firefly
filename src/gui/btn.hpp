# ifndef __btn__hpp
# define __btn__hpp
# include "btn_manager.hpp"
# include "../types/btn_info_t.hpp"
# include "../graphics/png_loader.hpp"
# include "../types/pixmap_t.h"
# include <boost/cstdint.hpp>
# include "../system/errno.h"
# include "../types/dsize_t.hpp"
namespace mdl {
namespace firefly {
namespace gui {
class btn {
	public:
		boost::int8_t fload_btn_pm(char const *__filedir, char const *__filename) {
			if (graphics::load_png_file(__filedir, __filename, this-> pixmap, this-> pm_size) != FFLY_SUCCESS) {
				fprintf(stderr, "btn: failed to load btn pixmap from file.\n");
				return FFLY_FAILURE;
			}
			return FFLY_SUCCESS;
		}

		boost::int8_t create_btn(uint_t& __btn_id, btn_manager *__btn_manager, types::coords_t<> __coords) {
			__btn_id = __btn_manager-> create_btn(this-> pixmap, __coords, this-> pm_size.xaxis_len, this-> pm_size.yaxis_len);
			return FFLY_SUCCESS;
		}
	private:
	types::dsize_t pm_size;
	types::pixmap_t pixmap = nullptr;
};
}
}
}

# endif /*__btn__hpp*/
