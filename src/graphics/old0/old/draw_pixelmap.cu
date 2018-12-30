# include "draw_pixelmap.hpp"
# include "../types/byte_t.h"
# include "colour_attr.hpp"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../ffly_def.h"
# include "../system/errno.h"
# include "../system/io.h"
__global__ void static draw_pixelmap(mdl::uint_t *__xfs, mdl::uint_t *__yfs, mdl::firefly::types::pixelmap_t __pixelbuff, mdl::uint_t *__pb_xa_len, mdl::uint_t *__pm_xfs, mdl::firefly::types::pixelmap_t __pixelmap, mdl::uint_t *__pm_xa_len, mdl::u16_t *__angle) {
	mdl::firefly::types::byte_t *src = __pixelmap+(((threadIdx.x+*__pm_xfs)+(blockIdx.x**__pm_xa_len))*4);
	mdl::firefly::types::byte_t *buf = __pixelbuff+(((threadIdx.x+*__xfs)+(blockIdx.x**__pb_xa_len))*4);

	mdl::firefly::types::byte_t alpha = *(src+A_OFF);
	mdl::firefly::types::byte_t invr_alpha = ~*(src+A_OFF);

	if (*(src+A_OFF) != 0x0) {
		mdl::firefly::types::byte_t r = (alpha**(src+R_OFF)+invr_alpha**(buf+R_OFF))>>8;
		mdl::firefly::types::byte_t g = (alpha**(src+G_OFF)+invr_alpha**(buf+G_OFF))>>8;
		mdl::firefly::types::byte_t b = (alpha**(src+B_OFF)+invr_alpha**(buf+B_OFF))>>8;
		*(buf+R_OFF) = r, *(buf+G_OFF) = g, *(buf+B_OFF) = b, *(buf+A_OFF) = *(src+A_OFF);
	}
}

mdl::firefly::types::err_t mdl::firefly::graphics::gpu_draw_pixelmap(gpu_dr_pm_mem& __gpu_dr_pm_mem, uint_t __xfs, uint_t __yfs, types::pixelmap_t __pixelbuff, uint_t __pb_xa_len, uint_t __pb_ya_len, uint_t __pm_xfs, types::pixelmap_t __pixelmap, uint_t __pm_xa_len, uint_t __pm_ya_len, uint_t __rpm_xa_len, u16_t __angle) {
	types::err_t any_err;
	uint_t pixelbuff_size = __pm_ya_len*__pb_xa_len*4;
	uint_t pixelmap_size = __pm_ya_len*__rpm_xa_len*4;
	if (!__gpu_dr_pm_mem.inited) {
		if ((any_err = memory::gpu_mem_alloc((void**)&__gpu_dr_pm_mem.xfs, sizeof(uint_t))) != FFLY_SUCCESS) {
			system::io::printf(stderr, "draw_pixelmap: cuda, failed to alloc memory for 'xfs', cu_errno: %d\n", any_err);
			ffly_errno = FF_ERR_CFTAM;
			return FFLY_FAILURE;
		}

		if ((any_err = memory::gpu_mem_alloc((void**)&__gpu_dr_pm_mem.yfs, sizeof(uint_t))) != FFLY_SUCCESS) {
			system::io::printf(stderr, "draw_pixelmap: cuda, failed to alloc memory for 'yfs', cu_errno: %d\n", any_err);
			ffly_errno = FF_ERR_CFTAM;
			return FFLY_FAILURE;
		}

		if ((any_err = memory::gpu_mem_alloc((void**)&__gpu_dr_pm_mem.pb_xa_len, sizeof(uint_t))) != FFLY_SUCCESS) {
			system::io::printf(stderr, "draw_pixelmap: cuda, failed to alloc memory for 'pb_xa_len', cu_errno: %d\n", any_err);
			ffly_errno = FF_ERR_CFTAM;
			return FFLY_FAILURE;
		}

		if ((any_err = memory::gpu_mem_alloc((void**)&__gpu_dr_pm_mem.pm_xa_len, sizeof(uint_t))) != FFLY_SUCCESS) {
			system::io::printf(stderr, "draw_pixelmap: cuda, failed to alloc memory for 'pm_xa_len', cu_errno: %d\n", any_err);
			ffly_errno = FF_ERR_CFTAM;
			return FFLY_FAILURE;
		}

		if ((any_err = memory::gpu_mem_alloc((void**)&__gpu_dr_pm_mem.pm_xfs, sizeof(uint_t))) != FFLY_SUCCESS) {
			system::io::printf(stderr, "draw_pixelmap: cuda, failed to alloc memory for 'pm_xfs', cu_errno: %d\n", any_err);
			ffly_errno = FF_ERR_CFTAM;
			return FFLY_FAILURE;
		}

		if ((any_err = memory::gpu_mem_alloc((void**)&__gpu_dr_pm_mem.angle, sizeof(u16_t))) != FFLY_SUCCESS) {
			system::io::printf(stderr, "draw_pixelmap: cuda, failed to alloc memory for 'angle', cu_errno: %d\n", any_err);
			ffly_errno = FF_ERR_CFTAM;
			return FFLY_FAILURE;
		}

		if (__xfs == 0) {
			if ((any_err = cudaMemcpy(__gpu_dr_pm_mem.xfs, &__xfs, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				system::io::printf(stderr, "draw_pixelmap: cuda, failed to copy memory to device, cu_errno: %d\n", any_err);
				ffly_errno = FF_ERR_CFTCM;
				return FFLY_FAILURE;
			}
		}

		if (__yfs == 0) {
			if ((any_err = cudaMemcpy(__gpu_dr_pm_mem.yfs, &__yfs, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				system::io::printf(stderr, "draw_pixelmap: cuda, failed to copy memory to device, cu_errno: %d\n", any_err);
				ffly_errno = FF_ERR_CFTCM;
				return FFLY_FAILURE;
			}
		}

		if (__pm_xfs == 0) {
			if ((any_err = cudaMemcpy(__gpu_dr_pm_mem.pm_xfs, &__pm_xfs, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				system::io::printf(stderr, "draw_pixelmap: cuda, failed to copy memory to device, cu_errno: %d\n", any_err);
				ffly_errno = FF_ERR_CFTCM;
				return FFLY_FAILURE;
			}
		}

		if (__angle == 0) {
			if ((any_err = cudaMemcpy(__gpu_dr_pm_mem.angle, &__angle, sizeof(u16_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
				system::io::printf(stderr, "draw_pixelmap: cuda, failed to copy memory to device, cu_errno: %d\n", any_err);
				ffly_errno = FF_ERR_CFTCM;
				return FFLY_FAILURE;
			}
		}
		__gpu_dr_pm_mem.inited = ffly_true;
	}

	if (__gpu_dr_pm_mem._pb_size != pixelbuff_size)
	{
		if (__gpu_dr_pm_mem.pixelbuff != nullptr) cudaFree(__gpu_dr_pm_mem.pixelbuff);
		if ((any_err = memory::gpu_mem_alloc((void**)&__gpu_dr_pm_mem.pixelbuff, pixelbuff_size*sizeof(types::__pixelmap_t))) != cudaSuccess) {
			system::io::printf(stderr, "cuda: failed to call Malloc, cu_errno: %d\n", any_err);
			ffly_errno = FF_ERR_CFTAM;
			return FFLY_FAILURE;
		}
		__gpu_dr_pm_mem._pb_size = pixelbuff_size;
	}

	if (__gpu_dr_pm_mem._pm_size != pixelmap_size)
	{
		if (__gpu_dr_pm_mem.pixelmap != nullptr) cudaFree(__gpu_dr_pm_mem.pixelmap);
		if ((any_err = memory::gpu_mem_alloc((void**)&__gpu_dr_pm_mem.pixelmap, pixelmap_size*sizeof(types::__pixelmap_t))) != cudaSuccess) {
			system::io::printf(stderr, "cuda: failed to call Malloc, cu_errno: %d\n", any_err);
			ffly_errno = FF_ERR_CFTAM;
			return FFLY_FAILURE;
		}
		__gpu_dr_pm_mem._pm_size = pixelmap_size;
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

	if (__gpu_dr_pm_mem._pb_xa_len != __pb_xa_len)
	{
		if ((any_err = cudaMemcpy(__gpu_dr_pm_mem.pb_xa_len, &__pb_xa_len, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			system::io::printf(stderr, "cuda: failed to call Memcpy, cu_errno: %d\n", any_err);
			ffly_errno = FF_ERR_CFTCM;
			return FFLY_FAILURE;
		}
		__gpu_dr_pm_mem._pb_xa_len = __pb_xa_len;
	}

	if (__gpu_dr_pm_mem._pm_xa_len != __rpm_xa_len)
	{
		if ((any_err = cudaMemcpy(__gpu_dr_pm_mem.pm_xa_len, &__rpm_xa_len, sizeof(uint_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
			system::io::printf(stderr, "cuda: failed to call Memcpy, cu_errno: %d\n", any_err);
			ffly_errno = FF_ERR_CFTCM;
			return FFLY_FAILURE;
		}
		__gpu_dr_pm_mem._pm_xa_len = __rpm_xa_len;
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

	uint_t pixelbuff_off = __yfs >= (__pb_ya_len-__pm_ya_len)? ((__pb_ya_len-__pm_ya_len)*__pb_xa_len)*4:(__yfs*__pb_xa_len)*4;
	if ((any_err = cudaMemcpy(__gpu_dr_pm_mem.pixelbuff, __pixelbuff+pixelbuff_off, pixelbuff_size*sizeof(types::__pixelmap_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
		system::io::printf(stderr, "cuda: failed to call Memcpy, cu_errno: %d\n", any_err);
		ffly_errno = FF_ERR_CFTCM;
		return FFLY_FAILURE;
	}

	if ((any_err = cudaMemcpy(__gpu_dr_pm_mem.pixelmap, __pixelmap, pixelmap_size*sizeof(types::__pixelmap_t), cudaMemcpyHostToDevice)) != cudaSuccess) {
		system::io::printf(stderr, "cuda: failed to call Memcpy, cu_errno: %d\n", any_err);
		ffly_errno = FF_ERR_CFTCM;
		return FFLY_FAILURE;
	}

	draw_pixelmap<<<__pm_ya_len, __pm_xa_len>>>(__gpu_dr_pm_mem.xfs, __gpu_dr_pm_mem.yfs, __gpu_dr_pm_mem.pixelbuff, __gpu_dr_pm_mem.pb_xa_len, __gpu_dr_pm_mem.pm_xfs, __gpu_dr_pm_mem.pixelmap, __gpu_dr_pm_mem.pm_xa_len, __gpu_dr_pm_mem.angle);
	if ((any_err = cudaDeviceSynchronize()) != cudaSuccess) {
		system::io::printf(stderr, "cuda: failed to sync device, cu_errno: %d\n", any_err);
		ffly_errno = FF_ERR_CFTSD;
		return FFLY_FAILURE;
	}

	if ((any_err = cudaMemcpy(__pixelbuff+pixelbuff_off, __gpu_dr_pm_mem.pixelbuff, pixelbuff_size*sizeof(types::__pixelmap_t), cudaMemcpyDeviceToHost)) != cudaSuccess) {
		system::io::printf(stderr, "cuda: failed to call Memcpy, cu_errno: %d\n", any_err);
		ffly_errno = FF_ERR_CFTCM;
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}
