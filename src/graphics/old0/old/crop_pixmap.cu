# include "crop_pixmap.hpp"
__global__ void cu_crop_pixmap(mdl::uint_t *__xfs, mdl::firefly::types::pixmap_t __pixbuff, mdl::firefly::types::pixmap_t __pixmap, mdl::uint_t *__pm_xlen, mdl::u8_t *__chn_c) {
	mdl::uint_t pb_point = (threadIdx.x + (blockIdx.x * blockDim.x)) * *__chn_c;
	mdl::uint_t pm_point = (threadIdx.x + *__xfs + (blockIdx.x * *__pm_xlen)) * *__chn_c;
	for (mdl::u8_t chn{}; chn != *__chn_c; chn ++) __pixbuff[pb_point + chn] = __pixmap[pm_point + chn];
}

__global__ void cu_crop_2d_pm(mdl::firefly::types::_1d_pm_t __pixbuff, mdl::firefly::types::_1d_pm_t __pixmap, mdl::u8_t *__chn_c, std::size_t __pb_pitch, std::size_t __pm_pitch) {
	mdl::firefly::types::_1d_pm_t pb_row = __pixbuff + blockIdx.x * __pb_pitch;
	mdl::firefly::types::_1d_pm_t pm_row = __pixmap + blockIdx.x * __pm_pitch;

	for (mdl::u8_t chn{}; chn != *__chn_c; chn ++) pb_row[(threadIdx.x * *__chn_c) + chn] = pm_row[(threadIdx.x * *__chn_c) + chn];
}
/* this need checking as i think Memcpy might go out of bounds */
mdl::firefly::types::err_t mdl::firefly::graphics::gpu_crop_2d_pm(uint_t __xfs, uint_t __yfs, types::_2d_pm_t __pixbuff, uint_t __pb_xlen, uint_t __pb_ylen, uint_t __pb_rxlen,
	types::_2d_pm_t __pixmap, uint_t __pm_xlen, uint_t __pm_ylen, uint_t __pm_rxlen, uint_t __pb_xfs, uint_t __pb_yfs, u8_t __chn_c) {

	bool static inited = false;
	types::_1d_pm_t static pixbuff = nullptr, pixmap = nullptr;
//	uint_t static *xfs = nullptr;
	u8_t static *chn_c = nullptr;

	if (__xfs + __pb_xlen > __pm_xlen) {
		uint_t sub = (__xfs + __pb_xlen) - __pm_xlen;
		__pm_xlen -= sub;
		__pb_xlen -= sub;
		__xfs -= __xfs >= sub? sub : 0;
		if (__pb_xfs >= sub) return FFLY_SUCCESS;
	}

	if (__yfs + __pb_ylen > __pm_ylen) {
		uint_t sub = (__yfs + __pb_ylen) - __pm_ylen;
		__pm_ylen -= sub;
		__pb_ylen -= sub;
		__yfs -= __yfs >= sub? sub : 0;
		if (__pb_yfs >= sub) return FFLY_SUCCESS;
	}

	cudaError_t any_err = cudaSuccess;
	uint_t pb_size = __pb_xlen * __pb_ylen * __chn_c;
	uint_t pm_size = __pm_xlen * __pm_ylen * __chn_c;
	if (!inited) {
		if ((any_err = cudaMalloc((void **)&chn_c, sizeof(u8_t))) != cudaSuccess) {
			fprintf(stderr, "crop_pixmap: cuda, failed to alloc memory for 'chn_c', errno: %d\n", any_err);
			return FFLY_FAILURE;
		}

		if (__chn_c == 0) {
			if ((any_err = cudaMemcpy(chn_c, &__chn_c, sizeof(u8_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				fprintf(stderr, "crop_pixmap: cuda, failed to copy memory for 'chn_c' to device, errno: %d\n", any_err);
				return FFLY_FAILURE;
			}
		}
		inited = true;
	}

	std::size_t static pb_pitch{};
	uint_t static _pb_size{};
	uint_t static _pb_xfs{}, _pb_yfs{};

	if (_pb_size != pb_size || _pb_xfs != __pb_xfs || _pb_yfs != __pb_yfs) {
		printf("cudaMalloc pixbuff\n");
		if (pixbuff != nullptr) {cudaFree(pixbuff);pb_pitch = 0x0;}
		if ((any_err = cudaMallocPitch((void **)&pixbuff, &pb_pitch, (__pb_xlen - __pb_xfs) * __chn_c * sizeof(types::__pixmap_t), __pb_ylen - __pb_yfs)) != cudaSuccess) {
			fprintf(stderr, "crop_pixmap: cuda, failed to alloc memory for pixbuff, errno: %d\n", any_err);
			return FFLY_FAILURE;
		}
		_pb_size = pb_size;
		_pb_xfs = __pb_xfs;
		_pb_yfs = __pb_yfs;
	}

	std::size_t static pm_pitch{};
	uint_t static _pb_xlen{}, _pb_ylen{};
	if (_pb_xlen != __pb_xlen || _pb_ylen != __pb_ylen) {
		printf("cudaMalloc pixmap\n");
		if (pixmap != nullptr) {cudaFree(pixmap);pm_pitch = 0x0;}
 		if ((any_err = cudaMallocPitch((void**)&pixmap, &pm_pitch, __pb_xlen * __chn_c * sizeof(types::__pixmap_t), __pb_ylen)) != cudaSuccess) {
			fprintf(stderr, "crop_pixmap: cuda, failed to alloc memory for pixmap, errno: %d\n", any_err);
			return FFLY_FAILURE;
		}
		_pb_xlen = __pb_xlen;
		_pb_ylen = __pb_ylen;
	}

	u8_t static _chn_c{};
	if (_chn_c != __chn_c) {
		if ((any_err = cudaMemcpy(chn_c, &__chn_c, sizeof(u8_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			fprintf(stderr, "crop_pixmap: cuda, failed to copy memory for 'chn_c' to device, errno: %d\n", any_err);
			return FFLY_FAILURE;
		}
		_chn_c = __chn_c;
	}

	//printf("ehhehehehehehehheh.\n");
	printf("%d - %d\n", __xfs, __yfs);
	if ((any_err = cudaMemcpy2D(pixbuff, pb_pitch, __pixbuff[__pb_yfs] + (__pb_xfs * __chn_c), __pb_rxlen * __chn_c, (__pb_xlen - __pb_xfs) * __chn_c, __pb_ylen - __pb_yfs, cudaMemcpyHostToDevice)) != cudaSuccess) {
		fprintf(stderr, "crop_pixmap: cuda, failed to copy memory for 'pixbuff' to device, errno: %d\n", any_err);
		return FFLY_FAILURE;
	}

	//printf("%d - %d - %d - %d - %d\n", __pm_rxlen * __chn_c, (__pm_xlen - __xfs) * __chn_c, pb_pitch, __yfs, __pm_ylen - __yfs);
	if ((any_err = cudaMemcpy2D(pixmap, pm_pitch, __pixmap[__yfs] + (__xfs * __chn_c), __pm_rxlen * __chn_c, __pb_xlen * __chn_c, __pb_ylen, cudaMemcpyHostToDevice)) != cudaSuccess) {
		fprintf(stderr, "crop_pixmap: cuda, failed to copy memory for 'pixmap' to device, errno: %d\n", any_err);
		return FFLY_FAILURE;
	}

	cu_crop_2d_pm<<<__pb_ylen - __pb_yfs, __pb_xlen - __pb_xfs>>>(pixbuff, pixmap, chn_c, pb_pitch, pm_pitch);
	cudaDeviceSynchronize();

	if ((any_err = cudaMemcpy2D(__pixbuff[__pb_yfs] + (__pb_xfs * __chn_c), __pb_rxlen * __chn_c, pixbuff, pb_pitch, (__pb_xlen - __pb_xfs) * __chn_c, __pb_ylen - __pb_yfs, cudaMemcpyDeviceToHost)) != cudaSuccess) {
		fprintf(stderr, "crop_pixmap: cuda, failed to copy memory for 'pixmap' to host, errno: %d\n", any_err);
	}

	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::graphics::gpu_crop_pixmap(uint_t __xfs, uint_t __yfs, types::pixmap_t __pixbuff, uint_t __pb_xlen, uint_t __pb_ylen, types::pixmap_t __pixmap, uint_t __pm_xlen, uint_t __pm_ylen, u8_t __chn_c) {
	bool static inited = false;
	types::pixmap_t static pixbuff = nullptr, pixmap = nullptr;
	uint_t static *xfs = nullptr, *pm_xlen;
	u8_t static *chn_c = nullptr;

	cudaError_t any_err = cudaSuccess;
	uint_t pb_size = __pb_xlen * __pb_ylen * __chn_c;
	uint_t pm_size = __pb_ylen * __pm_xlen * __chn_c;
	if (!inited) {
		if ((any_err = cudaMalloc((void **)&xfs, sizeof(uint_t))) != cudaSuccess) {
			fprintf(stderr, "crop_pixmap: cuda, failed to alloc memory for 'xfs', errno: %d\n", any_err);
			return FFLY_FAILURE;
		}

		if ((any_err = cudaMalloc((void **)&pm_xlen, sizeof(uint_t))) != cudaSuccess) {
			fprintf(stderr, "crop_pixmap: cuda, failed to alloc memory for 'pb_xlen', errno: %d\n", any_err);
			return FFLY_FAILURE;
		}

		if ((any_err = cudaMalloc((void **)&chn_c, sizeof(u8_t))) != cudaSuccess) {
			fprintf(stderr, "crop_pixmap: cuda, failed to alloc memory for 'chn_c', errno: %d\n", any_err);
			return FFLY_FAILURE;
		}

		if (__xfs == 0) {
			if ((any_err = cudaMemcpy(xfs, &__xfs, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				fprintf(stderr, "crop_pixmap: cuda, failed to copy memory for 'xfs' to device, errno: %d\n", any_err);
				return FFLY_FAILURE;
			}
		}

		if (__chn_c == 0) {
			if ((any_err = cudaMemcpy(chn_c, &__chn_c, sizeof(u8_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				fprintf(stderr, "crop_pixmap: cuda, failed to copy memory for 'chn_c' to device, errno: %d\n", any_err);
				return FFLY_FAILURE;
			}
		}
		inited = true;
	}

	uint_t static _pb_size{};
	if (_pb_size != pb_size)
	{
		if (pixbuff != nullptr) cudaFree(pixbuff);
		if ((any_err = cudaMalloc((void **)&pixbuff, pb_size * sizeof(types::__pixmap_t))) != cudaSuccess) {
			fprintf(stderr, "crop_pixmap: cuda, failed to alloc memory for 'pixbuff', errno: %d\n", any_err);
			return FFLY_FAILURE;
		}
		_pb_size = pb_size;
	}

	uint_t static _pm_size{};
	if (_pm_size != pm_size)
	{
		if (pixmap != nullptr) cudaFree(pixmap);
		if ((any_err = cudaMalloc((void **)&pixmap, pm_size * sizeof(types::__pixmap_t))) != cudaSuccess) {
			fprintf(stderr, "crop_pixmap: cuda, failed to alloc memory for 'pixmap', errno: %d\n", any_err);
			return FFLY_FAILURE;
		}
		_pm_size = pm_size;
	}

	uint_t static _xfs{};
	if (_xfs != __xfs) {
		if ((any_err = cudaMemcpy(xfs, &__xfs, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			fprintf(stderr, "crop_pixmap: cuda, failed to copy memory to device, errno: %d\n", any_err);
			return FFLY_FAILURE;
		}
		_xfs = __xfs;
	}

	uint_t static _pm_xlen{};
	if (_pm_xlen != __pm_xlen) {
		if ((any_err = cudaMemcpy(pm_xlen, &__pm_xlen, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			fprintf(stderr, "crop_pixmap: cuda, failed to copy memory for 'pb_xlen' to device, errno: %d\n", any_err);
			return FFLY_FAILURE;
		}
		_pm_xlen = __pm_xlen;
	}

	u8_t static _chn_c{};
	if (_chn_c != __chn_c) {
		if ((any_err = cudaMemcpy(chn_c, &__chn_c, sizeof(u8_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			fprintf(stderr, "crop_pixmap: cuda, failed to copy memory for 'chn_c' to device, errno: %d\n", any_err);
			return FFLY_FAILURE;
		}
		_chn_c = __chn_c;
	}

	if ((any_err = cudaMemcpy(pixbuff, __pixbuff, pb_size * sizeof(types::__pixmap_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
		fprintf(stderr, "crop_pixmap: cuda, failed to copy memory for 'pixbuff' to device, errno: %d\n", any_err);
		return FFLY_FAILURE;
	}
	uint_t pm_fs = __yfs >= (__pm_ylen - __pb_ylen)? ((__pm_ylen - __pb_ylen) * __pm_xlen) * __chn_c : __yfs * __pm_xlen * __chn_c;

	if ((any_err = cudaMemcpy(pixmap, __pixmap + pm_fs, pm_size * sizeof(types::__pixmap_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
		fprintf(stderr, "crop_pixmap: cuda, failed to copy memory for 'pixmap' to device, errno: %d\n", any_err);
		return FFLY_FAILURE;
	}

	cu_crop_pixmap<<<__pb_ylen, __pb_xlen>>>(xfs, pixbuff, pixmap, pm_xlen, chn_c);
	if ((any_err = cudaMemcpy(__pixbuff, pixbuff, pb_size * sizeof(types::__pixmap_t), cudaMemcpyDeviceToHost)) != cudaSuccess) {
		fprintf(stderr, "crop_pixmap: cuda, failed to copy memory for 'pixmap' to host, errno: %d\n", any_err);
		return FFLY_FAILURE;
	}

	return FFLY_SUCCESS;
}
