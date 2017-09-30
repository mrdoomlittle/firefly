# ifndef __asset__manager__hpp
# define __asset__manager__hpp
# include <mdlint.h>
# include <boost/array.hpp>
# include "graphics/skelmap_loader.hpp"
# include "types/skelmap_info_t.hpp"
# include <set>
# include "types/err_t.h"
# include "graphics/png_loader.hpp"
# include "types/dsize_t.hpp"
# include "types/pixmap_t.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/time_stamp.hpp"
# include "types/id_t.h"
# include "types/bool_t.h"
# include "types/asset_t.h"
# include "system/vec.h"
# include "types/byte_t.h"
# include "system/asset_kind.h"
# include "data/pair.h"
# include "types/aud_fad_t.h"
# include "ffly_audio.h"
namespace mdl {
namespace firefly {
class asset_manager
{
	public:
	enum {
		AST_PNG_FILE
	};

	types::err_t de_init();

	types::bool_t valid_asset_id(types::id_t __asset_id);
	types::id_t add_asset(types::byte_t *__data, uint_t __kind);
	types::id_t load_asset(char *__fdir, char *__fname, uint_t __kind);
	void del_asset(types::id_t __asset_id);
	u8_t* get_asset_data(types::id_t __asset_id);
	types::asset_t get_asset(types::id_t __asset_id) noexcept;
	types::asset_t& asset(types::id_t __asset_id);

	void* get_asset_info(types::id_t __asset_id);
	void*& asset_info(types::id_t _asset_id);

	private:
	uint_t amount_of_assets = 0;
	std::set<types::id_t> asset_ids;
	system::vec<types::asset_t> asset_indx;

};

}
}





# endif /*__asset__manager__hpp*/
