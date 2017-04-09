# ifndef __system__config__hpp
# define __system__config__hpp
# include <eint_t.hpp>
namespace mdl {
namespace firefly {
namespace system {
static struct {
	uint_t mn_workers = 2, mx_workers = 8;
	uint_t wk_chunk_xlen = 64;
	uint_t wk_chunk_ylen = 64;
	uint_t wk_chunk_zlen = 64;

	uint_t uni_par_xlen = 2;
	uint_t uni_par_ylen = 2;
	uint_t uni_par_zlen = 2;


} config;
}
}
}

# endif /*__system__config__hpp*/
