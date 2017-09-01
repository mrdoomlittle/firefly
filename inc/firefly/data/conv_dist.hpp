# ifndef __conv__dist__hpp
# define __conv__dist__hpp
# include <boost/cstdint.hpp>
namespace mdl {
namespace firefly {
namespace data {
template<typename _T, typename _B>
_T conv_dist(_B __b, _T __t, boost::uint8_t __umb, boost::uint8_t __utc);
}
}
}

# endif /*__conv__dist__hpp*/
