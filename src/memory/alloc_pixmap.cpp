# include "alloc_pixmap.hpp"
mdl::firefly::types::_1d_pm_t mdl::firefly::memory::alloc_pixmap(uint_t __xa_len, uint_t __ya_len, uint_t __za_len, u8_t __chn_c) {
	return static_cast<types::_1d_pm_t>(mem_alloc(((__xa_len * __ya_len * __za_len) * __chn_c) * sizeof(types::__pixmap_t)));
}
