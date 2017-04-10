# include "cuda_helper.hpp"
namespace mdl {
namespace firefly {
namespace graphics {
types::err_t get_device_c(uint_t& __device_c) {
	int device_c;
	if (cudaGetDeviceCount(&device_c) != cudaSuccess)
		return FFLY_FAILURE;
	__device_c = (uint_t)device_c;

	return FFLY_SUCCESS;
}
types::err_t max_threads(uint_t& __mx_threads) {
	uint_t device_c;

	if (get_device_c(device_c) == FFLY_FAILURE)
		return FFLY_FAILURE;
	struct cudaDeviceProp props;
	int device;
	for (device = 0; device < device_c; ++device) {
		cudaGetDeviceProperties(&props, device);
		if (device == 0) {
			__mx_threads = (uint_t)props.maxThreadsPerMultiProcessor;
		}
	}

	return FFLY_SUCCESS;
}
}
}
}
