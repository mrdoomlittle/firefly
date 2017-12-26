# ifndef __ffly__asset__manager__h
# define __ffly__asset__manager__h
# include "types/id_t.h"
# include "types/byte_t.h"
# include "types/asset_t.h"
# include "types/flag_t.h"
# include <mdlint.h>
# include "types/err_t.h"
# ifdef __cplusplus
extern "C" {
# endif
ffly_id_t ffly_load_asset(void*, char*, char*, mdl_uint_t, mdl_uint_t, ffly_err_t*);
ffly_id_t ffly_add_asset(void*, ffly_byte_t*, mdl_uint_t, ffly_err_t*);
ffly_asset_t* ffly_asset(void*, ffly_id_t);
# ifdef __cplusplus
}

# include "graphics/skelmap_loader.hpp"
# include "types/skelmap_info_t.hpp"
# include "types/size_t.h"
# include "types/dsize_t.hpp"
# include "types/pixmap_t.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/time_stamp.hpp"
# include "types/bool_t.h"
# include "system/vec.h"
# include "types/byte_t.h"
# include "system/asset.h"
# include "data/pair.h"
# include "types/aud_fad_t.h"
# include "types/mode_t.h"
# include "system/set.hpp"
namespace mdl {
namespace firefly {
class asset_manager
{
	public:
	enum {
		AST_NOLOAD
	};

	enum {
		AST_PNG_FILE
	};

	asset_manager() : asset_ids(*new system::set<types::id_t>), asset_d(*new system::vec<types::asset_t>) {}

	types::err_t de_init();

	types::bool_t valid_asset_id(types::id_t __asset_id);
	types::id_t add_asset(types::byte_t *__data, uint_t __kind, types::err_t& __err);
	types::id_t load_asset(char *__dir, char *__name, uint_t __kind, uint_t __format, types::mode_t __mode, types::err_t& __err);
	void del_asset(types::id_t __asset_id);
	u8_t* get_asset_data(types::id_t __asset_id);
	types::asset_t get_asset(types::id_t __asset_id) noexcept;
	types::asset_t& asset(types::id_t __asset_id);
	void free_asset(types::id_t __asset_id);
	void free_all();

	void* get_asset_info(types::id_t __asset_id);
	void*& asset_info(types::id_t _asset_id);

	private:
	uint_t no_assets = 0;
	system::set<types::id_t>& asset_ids;
	system::vec<types::asset_t>& asset_d;
};
}
}
# endif
# endif /*__ffly__asset__manager__h*/
