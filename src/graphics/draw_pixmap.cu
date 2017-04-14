# include "draw_pixmap.hpp"
__global__ void cu_draw_pixmap(mdl::uint_t *__xfs, mdl::uint_t *__yfs, mdl::firefly::types::pixmap_t __pixbuff, mdl::uint_t *__pb_xlen, mdl::uint_t *__pm_xfs, mdl::firefly::types::pixmap_t __pixmap, mdl::uint_t *__pm_xlen, mdl::u16_t *__angle) {
	mdl::uint_t pb_point = ((threadIdx.x + *__xfs) + (blockIdx.x * *__pb_xlen)) * 4;
	mdl::uint_t pm_point = ((threadIdx.x + *__pm_xfs) + (blockIdx.x * *__pm_xlen)) * 4;

	mdl::firefly::types::byte_t alpha = __pixmap[pm_point + 3];
	mdl::firefly::types::byte_t inv_alpha = 255 - __pixmap[pm_point + 3];

	if (__pixmap[pm_point + 3] != 0x0) {
		mdl::firefly::types::byte_t new_r = (mdl::firefly::types::byte_t)((alpha * __pixmap[pm_point] + inv_alpha * __pixbuff[pb_point]) >> 8);
		mdl::firefly::types::byte_t new_g = (mdl::firefly::types::byte_t)((alpha * __pixmap[pm_point + 1] + inv_alpha * __pixbuff[pb_point + 1]) >> 8);
		mdl::firefly::types::byte_t new_b = (mdl::firefly::types::byte_t)((alpha * __pixmap[pm_point + 2] + inv_alpha * __pixbuff[pb_point + 2]) >> 8);

		__pixbuff[pb_point] = new_r;
		__pixbuff[pb_point + 1] = new_g;
		__pixbuff[pb_point + 2] = new_b;

		__pixbuff[pb_point + 3] = __pixmap[pm_point + 3];
	}
}

mdl::firefly::types::err_t mdl::firefly::graphics::gpu_draw_pixmap(uint_t __xfs, uint_t __yfs, types::pixmap_t __pixbuff, uint_t __pb_xlen, uint_t __pb_ylen, uint_t __pm_xfs, types::pixmap_t __pixmap, uint_t __pm_xlen, uint_t __pm_ylen, uint_t __rpm_xlen, u16_t __angle) {
	uint_t static *xfs = nullptr, *yfs = nullptr, *pb_xlen = nullptr, *pm_xlen = nullptr, *pm_xfs;
	types::pixmap_t static pixbuff = nullptr, pixmap = nullptr;
	u16_t static *angle = nullptr;
	bool static inited = false;

	cudaError_t any_err = cudaSuccess;
	uint_t pb_size = __pm_ylen * __pb_xlen * 4;
	uint_t pm_size = __pm_ylen * __rpm_xlen * 4;
	if (!inited) {
		if ((any_err = cudaMalloc((void **)&xfs, sizeof(uint_t))) != cudaSuccess) {
			fprintf(stderr, "draw_pixmap: cuda, failed to alloc memory for 'xfs', errno: %d\n", any_err);
			return FFLY_FAILURE;
		}

		if ((any_err = cudaMalloc((void **)&yfs, sizeof(uint_t))) != cudaSuccess) {
			fprintf(stderr, "draw_pixmap: cuda, failed to alloc memory for 'yfs', errno: %d\n", any_err);
			return FFLY_FAILURE;
		}

		if ((any_err = cudaMalloc((void **)&pb_xlen, sizeof(uint_t))) != cudaSuccess) {
			fprintf(stderr, "draw_pixmap: cuda, failed to alloc memory for 'pb_xlen', errno: %d\n", any_err);
			return FFLY_FAILURE;
		}

		if ((any_err = cudaMalloc((void **)&pm_xlen, sizeof(uint_t))) != cudaSuccess) {
			fprintf(stderr, "draw_pixmap: cuda, failed to alloc memory for 'pm_xlen', errno: %d\n", any_err);
			return FFLY_FAILURE;
		}

		if ((any_err = cudaMalloc((void **)&pm_xfs, sizeof(uint_t))) != cudaSuccess) {
			fprintf(stderr, "draw_pixmap: cuda, failed to alloc memory for 'pm_xfs', errno: %d\n", any_err);
			return FFLY_FAILURE;
		}

		if ((any_err = cudaMalloc((void **)&angle, sizeof(u16_t))) != cudaSuccess) {
			fprintf(stderr, "draw_pixmap: cuda, failed to alloc memory for 'angle', errno: %d\n", any_err);
			return FFLY_FAILURE;
		}

		if (__xfs == 0) {
			if ((any_err = cudaMemcpy(xfs, &__xfs, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				fprintf(stderr, "draw_pixmap: cuda, failed to copy memory to device, errno: %d\n", any_err);
				return FFLY_FAILURE;
			}
		}

		if (__yfs == 0) {
			if ((any_err = cudaMemcpy(yfs, &__yfs, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				fprintf(stderr, "draw_pixmap: cuda, failed to copy memory to device, errno: %d\n", any_err);
				return FFLY_FAILURE;
			}
		}

		if (__pm_xfs == 0) {
			if ((any_err = cudaMemcpy(pm_xfs, &__pm_xfs, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				fprintf(stderr, "draw_pixmap: cuda, failed to copy memory to device, errno: %d\n", any_err);
				return FFLY_FAILURE;
			}
		}

		if (__angle == 0) {
			if ((any_err = cudaMemcpy(angle, &__angle, sizeof(u16_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				fprintf(stderr, "draw_pixmap: cuda, failed to copy memory to device, errno: %d\n", any_err);
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
			fprintf(stderr, "cuda: failed to call Malloc, errno: %d\n", any_err);
			return FFLY_FAILURE;
		}
		_pb_size = pb_size;
	}

	uint_t static _pm_size{};
	if (_pm_size != pm_size)
	{
		if (pixmap != nullptr) cudaFree(pixmap);
		if ((any_err = cudaMalloc((void **)&pixmap, pm_size * sizeof(types::__pixmap_t))) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Malloc, errno: %d\n", any_err);
			return FFLY_FAILURE;
		}
		_pm_size = pm_size;
	}

	uint_t static _xfs{};
	if (_xfs != __xfs) {
		if ((any_err = cudaMemcpy(xfs, &__xfs, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_err);
			return FFLY_FAILURE;
		}
		_xfs = __xfs;
	}

	uint_t static _yfs{};
	if (_yfs != __yfs) {
		if ((any_err = cudaMemcpy(yfs, &__yfs, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_err);
			return FFLY_FAILURE;
		}
		_yfs = __yfs;
	}

	uint_t static _pb_xlen{};
	if (_pb_xlen != __pb_xlen) {
		if ((any_err = cudaMemcpy(pb_xlen, &__pb_xlen, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_err);
			return FFLY_FAILURE;
		}
		_pb_xlen = __pb_xlen;
	}

	uint_t static _pm_xlen{};
	if (_pm_xlen != __rpm_xlen) {
		if ((any_err = cudaMemcpy(pm_xlen, &__rpm_xlen, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_err);
			return FFLY_FAILURE;
		}
		_pm_xlen = __rpm_xlen;
	}

	uint_t static _pm_xfs{};
	if (_pm_xfs != __pm_xfs) {
		if ((any_err = cudaMemcpy(pm_xfs, &__pm_xfs, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_err);
			return FFLY_FAILURE;
		}
		_pm_xfs = __pm_xfs;
	}

	u16_t static _angle = 0;
	if (_angle != __angle) {
		if ((any_err = cudaMemcpy(angle, &__angle, sizeof(u16_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_err);
			return FFLY_FAILURE;
		}

		_angle = __angle;
	}

	uint_t pb_fs = __yfs >= (__pb_ylen - __pm_ylen)? ((__pb_ylen - __pm_ylen) * __pb_xlen) * 4 : (__yfs * __pb_xlen) * 4;
	if ((any_err = cudaMemcpy(pixbuff, __pixbuff + pb_fs, pb_size * sizeof(types::__pixmap_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
		fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_err);
		return FFLY_FAILURE;
	}

	if ((any_err = cudaMemcpy(pixmap, __pixmap, pm_size * sizeof(types::__pixmap_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
		fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_err);
		return FFLY_FAILURE;
	}

	cu_draw_pixmap<<<__pm_ylen, __pm_xlen>>>(xfs, yfs, pixbuff, pb_xlen, pm_xfs, pixmap, pm_xlen, angle);
	if ((any_err = cudaMemcpy(__pixbuff + pb_fs, pixbuff, pb_size * sizeof(types::__pixmap_t), cudaMemcpyDeviceToHost)) != cudaSuccess) {
		fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_err);
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}
