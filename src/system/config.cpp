# include "config.hpp"
mdl::firefly::system::config_t mdl::firefly::system::config = {
	.mn_workers = 2, .mx_workers = 8,
	.wk_chunk_xlen = 64, .wk_chunk_ylen = 64, .wk_chunk_zlen = 64,
	.uni_par_xlen = 2, .uni_par_ylen = 2, .uni_par_zlen = 2,
	.mx_queue_size = 77
};
