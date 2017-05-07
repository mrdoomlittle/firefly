# ifndef __err__t__h
# define __err__t__h

# ifdef __cplusplus
# include <eint_t.hpp>

# ifdef __USING_OPENCL
#	include <CL/cl.hpp>
# endif
# ifdef __USING_CUDA
#	include <cuda_runtime.h>
# endif
namespace mdl {
namespace firefly {
namespace types {
typedef i8_t err_t;
# ifdef __USING_OPENCL
typedef cl_int cl_err_t;
# endif
# ifdef __USING_CUDA
typedef cudaError_t cu_err_t;
# endif
}
}
}
typedef mdl::i8_t ffly_err_t;
# else
# include <eint_t.h>
typedef mdl_i8_t ffly_err_t;
# endif

# endif /*__err__t__h*/
