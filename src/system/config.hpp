# ifndef __system__config__hpp
# define __system__config__hpp
# include <mdlint.h>
namespace mdl {
namespace firefly {
namespace system {
struct config_t {
	uint_t mn_workers, mx_workers;
	uint_t wk_chunk_xlen, wk_chunk_ylen, wk_chunk_zlen;
	uint_t uni_par_xlen, uni_par_ylen, uni_par_zlen;
	uint_t mx_queue_size;
};
config_t extern config;
}
}
}

# endif /*__system__config__hpp*/
