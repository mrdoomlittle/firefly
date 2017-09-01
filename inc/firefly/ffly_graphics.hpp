# ifndef __ffly__graphics__hpp
# define __ffly__graphics__hpp
# include <eint_t.hpp>
# include "types/err_t.h"
# define _CPU_MX_X 64
# define _CPU_MX_Y 64
namespace mdl {
namespace firefly {
namespace gpu {
uint_t extern device_c;
uint_t extern mx_threads;
uint_t extern mx_blocks;
}
}
}

# if defined(__USING_OPENCL) || defined(USING_OPENCL)
#	include "opencl_helper.hpp"
#	include <CL/cl.hpp>
# endif

# if defined(__USING_CUDA) || defined(USING_CUDA)
#	include "cuda_helper.hpp"
# endif
# include "types/pixmap_t.h"
# include <emu3d.hpp>
# include "system/buff.h"
# include "system/vec.h"
# include "data/pair.hpp"
namespace mdl {
namespace firefly {
namespace graphics {
namespace dev {
struct task {
	void(*call)(void*);
	void *arg;
};

types::err_t draw_pix_map(types::_1d_pm_t __pix_buff, uint_t __pb_xa_len, uint_t __pb_ya_lem, types::_1d_pm_t __pix_map, uint_t __pm_xa_len, uint_t __pm_ya_len, uint_t __xa_fs, uint_t __ya_fs);
}

void __inline__ itr_rgb_set(u8_t __r, u8_t __g, u8_t __b, types::_1d_pm_t __itr) {
	__itr[0] = __r;
    __itr[1] = __g;
    __itr[2] = __b;
}

void __inline__ itr_rgba_set(u8_t __r, u8_t __g, u8_t __b, u8_t __a, types::_1d_pm_t __itr) {
	itr_rgb_set(__r, __g, __b, __itr); __itr[3] = __a;}

void __inline__ rgb_set_pm_pix(u8_t __r, u8_t __g, u8_t __b, types::_1d_pm_t __pm, uint_t __xa, uint_t __ya, uint_t __za, uint_t __xa_len, uint_t __ya_len) {
	itr_rgb_set(__r, __g, __b, __pm+emu3d(__xa, __ya, __za, __xa_len, __ya_len, 0, 3));}

void __inline__ rgba_set_pm_pix(u8_t __r, u8_t __g, u8_t __b, u8_t __a, types::_1d_pm_t __pm, uint_t __xa, uint_t __ya, uint_t __za, uint_t __xa_len, uint_t __ya_len) {
	itr_rgba_set(__r, __g, __b, __a, __pm+emu3d(__xa, __ya, __za, __xa_len, __ya_len, 0, 4));}

void __inline__ rgb_set_pm_pix(u8_t __r, u8_t __g, u8_t __b, types::_3d_pm_t __pm, uint_t __xa, uint_t __ya, uint_t __za) {
	itr_rgb_set(__r, __g, __b, &__pm[__za][__ya][__xa]);}
void __inline__ rgba_set_pm_pix(u8_t __r, u8_t __g, u8_t __b, u8_t __a, types::_3d_pm_t __pm, uint_t __xa, uint_t __ya, uint_t __za) {
	itr_rgba_set(__r, __g, __b, __a, &__pm[__za][__ya][__xa]);}
}
}

namespace ffly_graphics {
bool extern inited;
// move to cpp file
firefly::types::err_t static init() {
# if defined(__USING_CUDA) || defined(USING_CUDA)
	firefly::gpu::get_device_c(firefly::gpu::device_c);
	firefly::gpu::get_mx_threads(firefly::gpu::mx_threads);
	firefly::gpu::mx_blocks = _CU_MAX_BLOCKS;
# endif
	inited = true;
	return FFLY_SUCCESS;
}
firefly::types::err_t static de_init() {
	return FFLY_SUCCESS;
}
}
}

# endif /*__ffly__graphics__hpp*/
