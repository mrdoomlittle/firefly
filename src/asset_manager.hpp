# ifndef __asset__manager__hpp
# define __asset__manager__hpp
# include <eint_t.hpp>
# include <boost/numeric/ublas/vector.hpp>
# include <boost/array.hpp>
# include "graphics/skelmap_loader.hpp"
# include "types/skelmap_info_t.hpp"
# include <set>
# include "graphics/png_loader.hpp"
# include "types/dsize_t.hpp"
# include "types/pixmap_t.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/time_stamp.hpp"
namespace ublas = boost::numeric::ublas;
namespace mdl {
namespace firefly {
class asset_manager
{
	public:
	enum {
		AST_PNG_FILE
	};

	typedef struct {
		uint_t *id, type;

		uint_t bytes;
		boost::uint8_t *data;
		void *info;
	} asset_t;

	boost::int8_t de_init();

	bool valid_asset_id(uint_t *__asset_id);
	uint_t *add_asset(boost::uint8_t *__data, uint_t __bytes, uint_t __type);
	uint_t *load_asset(char const *__asset_file, uint_t __asset_type);
	void del_asset(uint_t *__asset_id);
	boost::uint8_t* get_asset_data(uint_t *__asset_id);
	asset_t get_asset(uint_t *__asset_id) noexcept;
	asset_t& asset(uint_t *__asset_id);

	void *get_asset_info(uint_t *__asset_id);
	void*& asset_info(uint_t *__asset_id);

	private:
	uint_t amount_of_assets = 0;
	std::set<uint_t *> asset_ids;
	ublas::vector<asset_t> asset_indx;

};

}
}





# endif /*__asset__manager__hpp*/
