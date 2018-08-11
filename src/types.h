# ifndef __ffly__types__h
# define __ffly__types__h
# include "linux/types.h"
# include "ffint.h"

# ifdef __cplusplus
# ifdef __ffly_use_opencl
#   include <CL/cl.hpp>
# endif

# ifdef __ffly_use_cuda
#   include <cuda_runtime.h>
# endif
# endif

# ifdef  __ffly_source
typedef ff_i8_t ff_err_t;
typedef ff_u8_t ff_byte_t;
typedef ff_u16_t ff_addr_t;
typedef ff_u32_t ff_off_t;
typedef ff_u32_t ff_size_t;
typedef ff_s32_t ff_ssize_t;
typedef ff_u8_t ff_bool_t;

// id
typedef ff_uint_t _ff_id_t;
typedef ff_uint_t __ff_id_t;
typedef ff_uint_t* ff_id_t;

typedef ff_u16_t ff_tid_t;

// atomic
typedef ff_u64_t ff_atomic_u64_t;
typedef ff_u64_t ff_atomic_u32_t;
typedef ff_u64_t ff_atomic_u16_t;
typedef ff_u64_t ff_atomic_u8_t;
typedef ff_u64_t ff_atomic_uint_t;

typedef ff_u8_t ff_mlock_t;

typedef ff_u8_t ff_flag_t;
typedef ff_u32_t ff_fd_t;

typedef ff_u8_t ff_cond_lock_t;
typedef ff_u32_t ff_cache_t;
typedef ff_u32_t ff_rs_t;
# endif


# ifdef __cplusplus
namespace ff {
typedef ff_err_t err_t;
typedef ff_byte_t byte_t;
typedef ff_off_t off_t;
typedef ff_addr_t addr_t;
typedef ff_size_t size_t;
typedef ff_ssize_t ssize_t;
typedef ff_bool_t bool_t;

# ifdef __ffly_use_opencl
typedef cl_int cl_err_t;
# endif
# ifdef __ffly_use_cuda
typedef cudaError_t cl_err_t;
# endif
}
# endif
# endif /*__ffly__types__h*/
