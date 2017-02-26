# ifndef __alloc__pixmap__hpp
# define __alloc__pixmap__hpp
# include <boost/cstdint.hpp>
# include <eint_t.hpp>
namespace mdl {
namespace firefly {
namespace memory {
boost::uint8_t* alloc_pixmap(uint_t __xlen, uint_t __ylen, uint_t __zlen);
}
}
}

# endif /*__alloc__pixmap__hpp*/
