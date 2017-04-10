# include "draw_pixmap.hpp"
__global__ void cu_draw_pixmap(mdl::uint_t *__xfs, mdl::uint_t *__yfs, mdl::firefly::types::pixmap_t __pixbuff, mdl::uint_t *__pb_xlen, mdl::uint_t *__pm_xfs, mdl::firefly::types::pixmap_t __pixmap, mdl::uint_t *__pm_xlen, boost::uint16_t *__angle) {
	mdl::uint_t pb_point = ((threadIdx.x + (*__xfs)) + (blockIdx.x * (*__pb_xlen))) * 4;
	mdl::uint_t pm_point = ((threadIdx.x + (*__pm_xfs)) + (blockIdx.x * (*__pm_xlen))) * 4;

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

// NOTE: not thread safe
mdl::firefly::types::err_t mdl::firefly::graphics::gpu_draw_pixmap(uint_t __xfs, uint_t __yfs, types::pixmap_t __pixbuff, uint_t __pb_xlen, uint_t __pb_ylen, uint_t __pm_xfs, types::pixmap_t __pixmap, uint_t __pm_xlen, uint_t __pm_ylen, uint_t __rpm_xlen, boost::uint16_t __angle) {
	static uint_t *xfs = nullptr, *yfs = nullptr, *pb_xlen = nullptr, *pm_xlen = nullptr, *pm_xfs;
	static types::pixmap_t pixbuff = nullptr, pixmap = nullptr;
	static boost::uint16_t *angle = nullptr;
	static bool inited = false;
	cudaError_t any_error = cudaSuccess;

	if (!inited) {
		if ((any_error = cudaMalloc((void **)&xfs, sizeof(uint_t))) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Malloc, errno: %d\n", any_error);
			return FFLY_FAILURE;
		}

		if ((any_error = cudaMalloc((void **)&yfs, sizeof(uint_t))) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Malloc, errno: %d\n", any_error);
			return FFLY_FAILURE;
		}

		if ((any_error = cudaMalloc((void **)&pb_xlen, sizeof(uint_t))) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Malloc, errno: %d\n", any_error);
			return FFLY_FAILURE;
		}

		if ((any_error = cudaMalloc((void **)&pm_xlen, sizeof(uint_t))) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Malloc, errno: %d\n", any_error);
			return FFLY_FAILURE;
		}

		if ((any_error = cudaMalloc((void **)&pm_xfs, sizeof(uint_t))) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Malloc, errno: %d\n", any_error);
			return FFLY_FAILURE;
		}

		if ((any_error = cudaMalloc((void **)&angle, sizeof(boost::uint16_t))) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Malloc, errno: %d\n", any_error);
			return FFLY_FAILURE;
		}

		// if the any of thease are 0 then it will skip the memcpy so in the kernel we could end up having some junk memory
		// that might cause an error.
		if (__xfs == 0) {
			if ((any_error = cudaMemcpy(xfs, &__xfs, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_error);
				return FFLY_FAILURE;
			}
		}

		if (__yfs == 0) {
			if ((any_error = cudaMemcpy(yfs, &__yfs, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_error);
				return FFLY_FAILURE;
			}
		}

		if (__pb_xlen == 0) {
			if ((any_error = cudaMemcpy(pb_xlen, &__pb_xlen, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_error);
				return FFLY_FAILURE;
			}
		}

		if (__rpm_xlen == 0) {
			if ((any_error = cudaMemcpy(pm_xlen, &__rpm_xlen, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_error);
				return FFLY_FAILURE;
			}
		}

		if (__pm_xfs == 0) {
			if ((any_error = cudaMemcpy(pm_xfs, &__pm_xfs, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_error);
				return FFLY_FAILURE;
			}
		}

		if (__angle == 0) {
			if ((any_error = cudaMemcpy(angle, &__angle, sizeof(boost::uint16_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_error);
				return FFLY_FAILURE;
			}
		}
		inited = true;
	}

	uint_t pixbuff_size = (__pm_ylen * __pb_xlen) * 4;
	uint_t pixmap_size = (__pm_ylen * __rpm_xlen) * 4;

	static uint_t _pixbuff_size = 0;
	if (_pixbuff_size != pixbuff_size) {
		if (pixbuff != nullptr) cudaFree(pixbuff);

		if ((any_error = cudaMalloc((void **)&pixbuff, pixbuff_size * sizeof(types::__pixmap_t))) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Malloc, errno: %d\n", any_error);
			return FFLY_FAILURE;
		}
		_pixbuff_size = pixbuff_size;
	}

	static uint_t _pixmap_size = 0;
	if (_pixmap_size != pixmap_size) {
		if (pixmap != nullptr) cudaFree(pixmap);

		if ((any_error = cudaMalloc((void **)&pixmap, pixmap_size * sizeof(types::__pixmap_t))) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Malloc, errno: %d\n", any_error);
			return FFLY_FAILURE;
		}
		_pixmap_size = pixmap_size;
	}

	static uint_t _xfs = 0;
	if (_xfs != __xfs) {
		if ((any_error = cudaMemcpy(xfs, &__xfs, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_error);
			return FFLY_FAILURE;
		}
		_xfs = __xfs;
	}

	static uint_t _yfs = 0;
	if (_yfs != __yfs) {
		if ((any_error = cudaMemcpy(yfs, &__yfs, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_error);
			return FFLY_FAILURE;
		}
		_yfs = __yfs;
	}

	static uint_t _pb_xlen = 0;
	if (_pb_xlen != __pb_xlen) {
		if ((any_error = cudaMemcpy(pb_xlen, &__pb_xlen, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_error);
			return FFLY_FAILURE;
		}
		_pb_xlen = __pb_xlen;
	}

	static uint_t _pm_xlen = 0;
	if (_pm_xlen != __rpm_xlen) {
		if ((any_error = cudaMemcpy(pm_xlen, &__rpm_xlen, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_error);
			return FFLY_FAILURE;
		}
		_pm_xlen = __rpm_xlen;
	}

	static uint_t _pm_xfs = 0;
	if (_pm_xfs != __pm_xfs) {
		if ((any_error = cudaMemcpy(pm_xfs, &__pm_xfs, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_error);
			return FFLY_FAILURE;
		}
		_pm_xfs = __pm_xfs;
	}

	static boost::uint16_t _angle = 0;
	if (_angle != __angle) {
		if ((any_error = cudaMemcpy(angle, &__angle, sizeof(boost::uint16_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_error);
			return FFLY_FAILURE;
		}

		_angle = __angle;
	}

	uint_t pb_offset = __yfs >= (__pb_ylen - __pm_ylen)? ((__pb_ylen - __pm_ylen) * __pb_xlen) * 4: (__yfs * __pb_xlen) * 4;
	if ((any_error = cudaMemcpy(pixbuff, __pixbuff + pb_offset, pixbuff_size * sizeof(types::__pixmap_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
		fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_error);
		return FFLY_FAILURE;
	}

	if ((any_error = cudaMemcpy(pixmap, __pixmap, pixmap_size * sizeof(types::__pixmap_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
		fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_error);
		return FFLY_FAILURE;
	}

	cu_draw_pixmap<<<__pm_ylen, __pm_xlen>>>(xfs, yfs, pixbuff, pb_xlen, pm_xfs, pixmap, pm_xlen, angle);
	if ((any_error = cudaMemcpy(__pixbuff + pb_offset, pixbuff, pixbuff_size * sizeof(types::__pixmap_t), cudaMemcpyDeviceToHost)) != cudaSuccess) {
		fprintf(stderr, "cuda: failed to call Memcpy, errno: %d\n", any_error);
		return FFLY_FAILURE;
	}

	return FFLY_SUCCESS;
}
