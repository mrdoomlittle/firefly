# include "alloc_pixmap.hpp"

boost::uint8_t* mdl::firefly::memory::alloc_pixmap(uint_t __xlen, uint_t __ylen, uint_t __zlen) {
	return static_cast<boost::uint8_t *>(malloc(((__xlen * __ylen * __zlen) * 4) * sizeof(boost::uint8_t)));
}
