# ifndef __cuda__helper__hpp
# define __cuda__helper__hpp
# include <eint_t.hpp>
# include <cuda_runtime.h>
# include "types/err_t.h"
# include "system/errno.h"
# define _CU_MAX_BLOCKS 65535
namespace mdl {
namespace firefly {
namespace gpu {
types::err_t get_device_c(uint_t& __device_c);
types::err_t get_mx_threads(uint_t& __mx_threads);
}
}
}

# endif /*__cuda__helper__hpp*/
