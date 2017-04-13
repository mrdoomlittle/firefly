# ifndef __ffly__graphics__hpp
# define __ffly__graphics__hpp
# include <eint_t.hpp>
# include "types/err_t.h"
# define _CPU_MX_X 64
# define _CPU_MX_Y 64
namespace mdl {
namespace firefly {
namespace gpu {
uint_t extern device_c;
uint_t extern mx_threads;
uint_t extern mx_blocks;
}
}
}
/*
find a diffrent name for mdl::firefly::gpu::init()
*/
# if defined(__USING_OPENCL) || defined(USING_OPENCL)
#	include "opencl_helper.hpp"
#	include <CL/cl.hpp>
# endif

# if defined(__USING_CUDA) || defined(USING_CUDA)
#	include "cuda_helper.hpp"
# endif

namespace mdl {
namespace ffly_graphics {
bool extern inited;
static firefly::types::err_t init() {
	firefly::gpu::get_device_c(firefly::gpu::device_c);
	firefly::gpu::get_mx_threads(firefly::gpu::mx_threads);
	firefly::gpu::mx_blocks = _CU_MAX_BLOCKS;
	inited = true;
}
}
}

# endif /*__ffly__graphics__hpp*/
