# include "uni_dlen_val.hpp"
mdl::uint_t mdl::firefly::data::uni_dlen_val(types::uni_dlen_t __uni_dlen) {
	switch(__uni_dlen) {
		case uni_dlen::UNI_DLEN_4: return 4;
		case uni_dlen::UNI_DLEN_8: return 8;
		case uni_dlen::UNI_DLEN_16: return 16;
		case uni_dlen::UNI_DLEN_32: return 32;
		case uni_dlen::UNI_DLEN_64: return 64;
		case uni_dlen::UNI_DLEN_128: return 128;
		case uni_dlen::UNI_DLEN_256: return 256;
		case uni_dlen::UNI_DLEN_512: return 512;
		case uni_dlen::UNI_DLEN_1024: return 1024;
		case uni_dlen::UNI_DLEN_2048: return 2048;
	}
}
