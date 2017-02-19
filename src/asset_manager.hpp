# ifndef __asset__manager__hpp
# define __asset__manager__hpp
# include <eint_t.hpp>
# define SKELMAP_T 0
# include <boost/numeric/ublas/vector.hpp>
# include <boost/array.hpp>
# include "graphics/skelmap_loader.hpp"
# include "types/skelmap_info_t.hpp"
namespace ublas = boost::numeric::ublas;
namespace mdl {
namespace firefly {
class asset_manager
{
	public:
	typedef struct {
		uint_t asset_id;
		uint_t asset_type;

		types::skelmap_info_t skelmap_info;
	} asset_info_t;

	~asset_manager() {
		for (std::size_t o = 0; o != this-> amount_of_assets; o ++)
			if (this-> asset_data[o] != nullptr) std::free(this-> asset_data[o]);
	}

	uint_t load_asset(char const *__asset_file, uint_t __asset_type);
	boost::uint8_t* get_asset_data(uint_t __asset_id);
	asset_info_t get_asset_info(uint_t __asset_id);
	private:
	uint_t amount_of_assets = 0;
	ublas::vector<boost::uint8_t *> asset_data;
	ublas::vector<asset_info_t> asset_info;

};

}
}





# endif /*__asset__manager__hpp*/
