# ifndef __btn__hpp
# define __btn__hpp
# include "btn_manager.hpp"
# include "../types/btn_info_t.hpp"
# include "../graphics/png_loader.hpp"
# include "../types/pixmap_t.h"
# include <boost/cstdint.hpp>
# include "../system/errno.h"
# include "../types/dsize_t.hpp"
# include "../types/btn_t.hpp"
# include "../maths/find_center.hpp"
# include "../asset_manager.hpp"
# include "../types/id_t.hpp"
# include "../types/err_t.h"
# include <initializer_list>
namespace mdl {
namespace firefly {
namespace gui {
class btn {
	public:
		types::err_t fload_btn_pm(char const *__filedir, char const *__filename) {
			if (graphics::load_png_file(__filedir, __filename, this-> pixmap, this-> pm_size) != FFLY_SUCCESS) {
				fprintf(stderr, "btn: failed to load btn pixmap from file.\n");
				return FFLY_FAILURE;
			}
			return FFLY_SUCCESS;
		}

		types::err_t load_btn_ast(types::id_t __asset_id, asset_manager *__asset_manager) {
			if (__asset_manager-> asset(__asset_id).type == asset_manager::AST_PNG_FILE) {
				this-> pm_size = *(types::_2d_dsize_t<> *)__asset_manager-> get_asset_info(__asset_id);
				this-> pixmap = __asset_manager-> get_asset_data(__asset_id);
			}
		}

		types::err_t create_btn(uint_t& __btn_id, btn_manager *__btn_manager, types::_2d_coords_t<> __coords) {
			__btn_id = __btn_manager-> create_btn(this-> pixmap, __coords, this-> pm_size.xaxis_len, this-> pm_size.yaxis_len);
			__btn_manager-> get_btn(__btn_id)-> inde_tx_mem = true;
			return FFLY_SUCCESS;
		}

		void center_btn(std::initializer_list<types::btn_t *> __btn_id, uint_t __xmax = 0, uint_t __ymax = 0) {
			for (std::initializer_list<types::btn_t *>::iterator itor = __btn_id.begin(); itor != __btn_id.end(); ++itor)
				this-> center_btn(*itor, __xmax, __ymax);
		}

		void center_btn(types::btn_t *__btn, uint_t __xmax = 0, uint_t __ymax = 0) {
			if (__xmax != 0)
				__btn-> coords.xaxis = maths::find_center(__xmax) - maths::find_center(__btn-> xaxis_len);

			if (__ymax != 0)
				__btn-> coords.yaxis = maths::find_center(__ymax) - maths::find_center(__btn-> yaxis_len);
		}

		types::_2d_dsize_t<> get_pm_size() {
			return pm_size;
		}
	private:
	types::_2d_dsize_t<> pm_size;
	types::pixmap_t pixmap = nullptr;
};
}
}
}

# endif /*__btn__hpp*/
