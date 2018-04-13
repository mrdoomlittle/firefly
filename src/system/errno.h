# ifndef __ffly__errno__h
# define __ffly__errno__h
# include "../ffint.h"
# ifdef __fflib
# include "../linux/errno.h"
# else
# include <errno.h>
# endif
# ifdef __ffly_use_opencl
#   include <CL/cl.hpp>
#	define ffly_cl_success CL_SUCCESS
# endif
# ifdef __ffly_use_cuda
#   include <cuda_runtime.h>
#	define ffly_cl_success cudaSuccess
# endif

# ifndef FFLY_SUCCESS
#	define FFLY_SUCCESS 0
# endif

# ifndef FFLY_FAILURE
#	define FFLY_FAILURE -1
# endif

# ifndef FFLY_NOP
#	define FFLY_NOP 1
# endif

# ifndef FF_ERR_NULL
#	define FF_ERR_NULL 0
# endif

# ifndef FF_ERR_CFTCM
#	define FF_ERR_CFTCM 1 /*cuda failed to copy memory*/
# endif

# ifndef FF_ERR_CFTAM
#	define FF_ERR_CFTAM 2 /*cuda failed to alloc memory*/
# endif

# ifndef FF_ERR_CFTCK
#	define FF_ERR_CFTCK 3 /*cuda failed to call kernel*/
# endif

# ifndef FF_ERR_CFTFM
#	define FF_ERR_CFTFM 4 /*cuda failed to free memory*/
# endif

# ifndef FF_ERR_CFTSD
#	define FF_ERR_CFTSD 5 /*cuda failed to sync device*/
# endif

# ifndef FF_ERR_CEBF
#	define FF_ERR_CEBF 6 /* core event buff full*/
# endif

# ifndef FF_ERR_SMBF
#	define FF_ERR_SMBF 7 /*static memory buff full*/
# endif

# ifndef FF_ERR_SMBE
#	define FF_ERR_SMBE 8 /*static memory buff empty*/
# endif

# ifndef FF_ERR_WDEQE
#	define FF_ERR_WDEQE 9 /* window event queue empty*/
# endif

# ifdef __cplusplus
extern "C" {
# endif
# ifdef __fflib
int extern errno;
char const* strerror(int);
# endif
ff_u8_t extern ffly_errno;
# ifdef __cplusplus
}
# endif
# endif /*__ffly__errno__h*/
