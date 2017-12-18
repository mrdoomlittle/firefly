# ifndef __ffly__err__t__h
# define __ffly__err__t__h
# include <mdlint.h>

# ifdef __cplusplus
# ifdef __ffly_use_opencl
#	include <CL/cl.hpp>
# endif

# ifdef __ffly_use_cuda
#	include <cuda_runtime.h>
# endif
# endif

typedef mdl_i8_t ffly_err_t;
# ifdef __cplusplus
namespace mdl {
namespace firefly {
namespace types {
typedef ffly_err_t err_t;
# ifdef __ffly_use_opencl
typedef cl_int cl_err_t;
# endif
# ifdef __ffly_use_cuda
typedef cudaError_t cl_err_t;
# endif
}
}
}
# endif
# endif /*__ffly__err__t__h*/
