# include "draw_pixmap.hpp"
__global__ void cu_draw_pixmap(mdl::uint_t *__xfs, mdl::uint_t *__yfs, mdl::firefly::types::pixmap_t __pixbuff, mdl::uint_t *__pb_xlen, mdl::uint_t *__pm_xfs, mdl::firefly::types::pixmap_t __pixmap, mdl::uint_t *__pm_xlen, mdl::u16_t *__angle) {
	mdl::uint_t pb_point = ((threadIdx.x+*__xfs)+(blockIdx.x**__pb_xlen))*4;
	mdl::uint_t pm_point = ((threadIdx.x+*__pm_xfs)+(blockIdx.x**__pm_xlen))*4;

	mdl::firefly::types::byte_t alpha = __pixmap[pm_point+3];
	mdl::firefly::types::byte_t inv_alpha = 255-__pixmap[pm_point+3];

	if (__pixmap[pm_point+3] != 0x0) {
		mdl::firefly::types::byte_t r = (mdl::firefly::types::byte_t)((alpha*__pixmap[pm_point]+inv_alpha*__pixbuff[pb_point]) >> 8);
		mdl::firefly::types::byte_t g = (mdl::firefly::types::byte_t)((alpha*__pixmap[pm_point+1]+inv_alpha*__pixbuff[pb_point+1]) >> 8);
		mdl::firefly::types::byte_t b = (mdl::firefly::types::byte_t)((alpha*__pixmap[pm_point+2]+inv_alpha*__pixbuff[pb_point+2]) >> 8);

		__pixbuff[pb_point] = r;
		__pixbuff[pb_point+1] = g;
		__pixbuff[pb_point+2] = b;
		__pixbuff[pb_point+3] = __pixmap[pm_point+3];
	}
}

mdl::firefly::types::err_t mdl::firefly::graphics::gpu_draw_pixmap(gpu_dr_pm_mem& __gpu_dr_pm_mem, uint_t __xfs, uint_t __yfs, types::pixmap_t __pixbuff, uint_t __pb_xlen, uint_t __pb_ylen, uint_t __pm_xfs, types::pixmap_t __pixmap, uint_t __pm_xlen, uint_t __pm_ylen, uint_t __rpm_xlen, u16_t __angle) {
	types::cu_err_t any_err = cudaSuccess;
	uint_t pb_size = __pm_ylen*__pb_xlen*4;
	uint_t pm_size = __pm_ylen*__rpm_xlen*4;
	if (!__gpu_dr_pm_mem.inited) {
		if ((any_err = cudaMalloc((void**)&__gpu_dr_pm_mem.xfs, sizeof(uint_t))) != cudaSuccess) {
			system::io::printf(stderr, "draw_pixmap: cuda, failed to alloc memory for 'xfs', cu_errno: %d\n", any_err);
			ffly_errno = FF_ERR_CFTAM;
			return FFLY_FAILURE;
		}

		if ((any_err = cudaMalloc((void**)&__gpu_dr_pm_mem.yfs, sizeof(uint_t))) != cudaSuccess) {
			system::io::printf(stderr, "draw_pixmap: cuda, failed to alloc memory for 'yfs', cu_errno: %d\n", any_err);
			ffly_errno = FF_ERR_CFTAM;
			return FFLY_FAILURE;
		}

		if ((any_err = cudaMalloc((void**)&__gpu_dr_pm_mem.pb_xlen, sizeof(uint_t))) != cudaSuccess) {
			system::io::printf(stderr, "draw_pixmap: cuda, failed to alloc memory for 'pb_xlen', cu_errno: %d\n", any_err);
			ffly_errno = FF_ERR_CFTAM;
			return FFLY_FAILURE;
		}

		if ((any_err = cudaMalloc((void**)&__gpu_dr_pm_mem.pm_xlen, sizeof(uint_t))) != cudaSuccess) {
			system::io::printf(stderr, "draw_pixmap: cuda, failed to alloc memory for 'pm_xlen', cu_errno: %d\n", any_err);
			ffly_errno = FF_ERR_CFTAM;
			return FFLY_FAILURE;
		}

		if ((any_err = cudaMalloc((void**)&__gpu_dr_pm_mem.pm_xfs, sizeof(uint_t))) != cudaSuccess) {
			system::io::printf(stderr, "draw_pixmap: cuda, failed to alloc memory for 'pm_xfs', cu_errno: %d\n", any_err);
			ffly_errno = FF_ERR_CFTAM;
			return FFLY_FAILURE;
		}

		if ((any_err = cudaMalloc((void**)&__gpu_dr_pm_mem.angle, sizeof(u16_t))) != cudaSuccess) {
			system::io::printf(stderr, "draw_pixmap: cuda, failed to alloc memory for 'angle', cu_errno: %d\n", any_err);
			ffly_errno = FF_ERR_CFTAM;
			return FFLY_FAILURE;
		}

		if (__xfs == 0) {
			if ((any_err = cudaMemcpy(__gpu_dr_pm_mem.xfs, &__xfs, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				system::io::printf(stderr, "draw_pixmap: cuda, failed to copy memory to device, cu_errno: %d\n", any_err);
				ffly_errno = FF_ERR_CFTCM;
				return FFLY_FAILURE;
			}
		}

		if (__yfs == 0) {
			if ((any_err = cudaMemcpy(__gpu_dr_pm_mem.yfs, &__yfs, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				system::io::printf(stderr, "draw_pixmap: cuda, failed to copy memory to device, cu_errno: %d\n", any_err);
				ffly_errno = FF_ERR_CFTCM;
				return FFLY_FAILURE;
			}
		}

		if (__pm_xfs == 0) {
			if ((any_err = cudaMemcpy(__gpu_dr_pm_mem.pm_xfs, &__pm_xfs, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				system::io::printf(stderr, "draw_pixmap: cuda, failed to copy memory to device, cu_errno: %d\n", any_err);
				ffly_errno = FF_ERR_CFTCM;
				return FFLY_FAILURE;
			}
		}

		if (__angle == 0) {
			if ((any_err = cudaMemcpy(__gpu_dr_pm_mem.angle, &__angle, sizeof(u16_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				system::io::printf(stderr, "draw_pixmap: cuda, failed to copy memory to device, cu_errno: %d\n", any_err);
				ffly_errno = FF_ERR_CFTCM;
				return FFLY_FAILURE;
			}
		}
		__gpu_dr_pm_mem.inited = true;
	}

	if (__gpu_dr_pm_mem._pb_size != pb_size)
	{
		if (__gpu_dr_pm_mem.pixbuff != nullptr) cudaFree(__gpu_dr_pm_mem.pixbuff);
		if ((any_err = cudaMalloc((void**)&__gpu_dr_pm_mem.pixbuff, pb_size*sizeof(types::__pixmap_t))) != cudaSuccess) {
			system::io::printf(stderr, "cuda: failed to call Malloc, cu_errno: %d\n", any_err);
			ffly_errno = FF_ERR_CFTAM;
			return FFLY_FAILURE;
		}
		__gpu_dr_pm_mem._pb_size = pb_size;
	}

	if (__gpu_dr_pm_mem._pm_size != pm_size)
	{
		if (__gpu_dr_pm_mem.pixmap != nullptr) cudaFree(__gpu_dr_pm_mem.pixmap);
		if ((any_err = cudaMalloc((void**)&__gpu_dr_pm_mem.pixmap, pm_size*sizeof(types::__pixmap_t))) != cudaSuccess) {
			system::io::printf(stderr, "cuda: failed to call Malloc, cu_errno: %d\n", any_err);
			ffly_errno = FF_ERR_CFTAM;
			return FFLY_FAILURE;
		}
		__gpu_dr_pm_mem._pm_size = pm_size;
	}

	if (__gpu_dr_pm_mem._xfs != __xfs)
	{
		if ((any_err = cudaMemcpy(__gpu_dr_pm_mem.xfs, &__xfs, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			system::io::printf(stderr, "cuda: failed to call Memcpy, cu_errno: %d\n", any_err);
			ffly_errno = FF_ERR_CFTCM;
			return FFLY_FAILURE;
		}
		__gpu_dr_pm_mem._xfs = __xfs;
	}

	if (__gpu_dr_pm_mem._yfs != __yfs)
	{
		if ((any_err = cudaMemcpy(__gpu_dr_pm_mem.yfs, &__yfs, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			system::io::printf(stderr, "cuda: failed to call Memcpy, cu_errno: %d\n", any_err);
			ffly_errno = FF_ERR_CFTCM;
			return FFLY_FAILURE;
		}
		__gpu_dr_pm_mem._yfs = __yfs;
	}

	if (__gpu_dr_pm_mem._pb_xlen != __pb_xlen)
	{
		if ((any_err = cudaMemcpy(__gpu_dr_pm_mem.pb_xlen, &__pb_xlen, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			system::io::printf(stderr, "cuda: failed to call Memcpy, cu_errno: %d\n", any_err);
			ffly_errno = FF_ERR_CFTCM;
			return FFLY_FAILURE;
		}
		__gpu_dr_pm_mem._pb_xlen = __pb_xlen;
	}

	if (__gpu_dr_pm_mem._pm_xlen != __rpm_xlen)
	{
		if ((any_err = cudaMemcpy(__gpu_dr_pm_mem.pm_xlen, &__rpm_xlen, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			system::io::printf(stderr, "cuda: failed to call Memcpy, cu_errno: %d\n", any_err);
			ffly_errno = FF_ERR_CFTCM;
			return FFLY_FAILURE;
		}
		__gpu_dr_pm_mem._pm_xlen = __rpm_xlen;
	}

	if (__gpu_dr_pm_mem._pm_xfs != __pm_xfs)
	{
		if ((any_err = cudaMemcpy(__gpu_dr_pm_mem.pm_xfs, &__pm_xfs, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			system::io::printf(stderr, "cuda: failed to call Memcpy, cu_errno: %d\n", any_err);
			ffly_errno = FF_ERR_CFTCM;
			return FFLY_FAILURE;
		}
		__gpu_dr_pm_mem._pm_xfs = __pm_xfs;
	}

	if (__gpu_dr_pm_mem._angle != __angle)
	{
		if ((any_err = cudaMemcpy(__gpu_dr_pm_mem.angle, &__angle, sizeof(u16_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			system::io::printf(stderr, "cuda: failed to call Memcpy, cu_errno: %d\n", any_err);
			ffly_errno = FF_ERR_CFTCM;
			return FFLY_FAILURE;
		}
		__gpu_dr_pm_mem._angle = __angle;
	}

	uint_t pb_fs = __yfs >= (__pb_ylen-__pm_ylen)? ((__pb_ylen-__pm_ylen)*__pb_xlen)*4 : (__yfs*__pb_xlen)*4;
	if ((any_err = cudaMemcpy(__gpu_dr_pm_mem.pixbuff, __pixbuff+pb_fs, pb_size*sizeof(types::__pixmap_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
		system::io::printf(stderr, "cuda: failed to call Memcpy, cu_errno: %d\n", any_err);
		ffly_errno = FF_ERR_CFTCM;
		return FFLY_FAILURE;
	}

	if ((any_err = cudaMemcpy(__gpu_dr_pm_mem.pixmap, __pixmap, pm_size*sizeof(types::__pixmap_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
		system::io::printf(stderr, "cuda: failed to call Memcpy, cu_errno: %d\n", any_err);
		ffly_errno = FF_ERR_CFTCM;
		return FFLY_FAILURE;
	}

	cu_draw_pixmap<<<__pm_ylen, __pm_xlen>>>(__gpu_dr_pm_mem.xfs, __gpu_dr_pm_mem.yfs, __gpu_dr_pm_mem.pixbuff, __gpu_dr_pm_mem.pb_xlen,
		__gpu_dr_pm_mem.pm_xfs, __gpu_dr_pm_mem.pixmap, __gpu_dr_pm_mem.pm_xlen, __gpu_dr_pm_mem.angle);
	if ((any_err = cudaDeviceSynchronize()) != cudaSuccess) {
		system::io::printf(stderr, "cuda: failed to sync device, cu_errno: %d\n", any_err);
		ffly_errno = FF_ERR_CFTSD;
		return FFLY_FAILURE;
	}

	if ((any_err = cudaMemcpy(__pixbuff+pb_fs, __gpu_dr_pm_mem.pixbuff, pb_size*sizeof(types::__pixmap_t), cudaMemcpyDeviceToHost)) != cudaSuccess) {
		system::io::printf(stderr, "cuda: failed to call Memcpy, cu_errno: %d\n", any_err);
		ffly_errno = FF_ERR_CFTCM;
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}
