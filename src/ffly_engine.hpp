# ifndef __ffly__engine__hpp
# define __ffly__engine__hpp
# include <boost/cstdint.hpp>
# include "system/errno.h"
# include <cstdio>

# ifdef USING_CUDA
# include <cuda_runtime.h>
__inline__ boost::int8_t ffly_check_for_gpu() {
	int device_count, device;

	cudaError_t any_error = cudaSuccess;

	if ((any_error = cudaGetDeviceCount(&device_count)) != cudaSuccess) {
		fprintf(stderr, "cuda: failed to fetch gpu count.\n");
		return FFLY_FAILURE;
	}

# ifdef FORCE_GPU
	if (device_count) {
		fprintf(stderr, "cuda: can't find any gpu? do you have any installed?\n");
		return FFLY_FAILURE;
	}
# else
	if (device_count)
		printf("seems like you have no gpu install, using the cpu might be extreamly slow.\n");
# endif
}

# endif

# ifdef USING_OPENCL


# endif

# if defined(FFLY_SERVER)
	# include "ffly_server.hpp"
# elif defined(FFLY_CLIENT)
	# include "ffly_client.hpp"
# endif
# endif /*__ffly__engine__hpp*/
