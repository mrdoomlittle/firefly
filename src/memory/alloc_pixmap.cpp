# include "alloc_pixmap.hpp"
mdl::firefly::types::pixmap_t mdl::firefly::memory::alloc_pixmap(uint_t __xlen, uint_t __ylen, uint_t __zlen, uint_t __block_len) {
	return static_cast<boost::uint8_t *>(mem_alloc(((__xlen * __ylen * __zlen) * __block_len) * sizeof(boost::uint8_t)));
}
