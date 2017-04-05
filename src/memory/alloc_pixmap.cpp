# include "alloc_pixmap.hpp"
mdl::firefly::types::pixmap_t mdl::firefly::memory::alloc_pixmap(uint_t __xaxis_len, uint_t __yaxis_len, uint_t __zaxis_len, uint_t __block_len) {
	return static_cast<types::pixmap_t>(mem_alloc(((__xaxis_len * __yaxis_len * __zaxis_len) * __block_len) * sizeof(types::__pixmap_t)));
}
