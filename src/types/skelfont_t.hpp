# ifndef __skelfont__t__hpp
# define __skelfont__t__hpp
# include <eint_t.hpp>
# include <utility>
# include <boost/numeric/ublas/vector.hpp>
namespace ublas = boost::numeric::ublas;
namespace mdl {
namespace firefly {
namespace types {
typedef struct {
	uint_t font_size;
	int_t font_data[255] = {-1};
//	std::unordered_map<char, uint_t> font_data;
	//ublas::vector<std::pair<char, uint_t>> font_data;
} skelfont_t;
}
}
}

# endif /*__skelfont__t__hpp*/
