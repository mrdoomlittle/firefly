# ifndef __graphics__draw__pixmap__hpp
# define __graphics__draw__pixmap__hpp
# include <eint_t.hpp>
# include <cstdio>
# include <math.h>
# include <boost/cstdint.hpp>
//# define USING_OPENCL
# if defined(__USING_OPENCL) || defined(USING_OPENCL)
#	include <CL/cl.hpp>
#	include "../opencl_helper.hpp"
# endif

# if defined(__USING_CUDA) || defined(USING_CUDA)
#	include "../cuda_helper.hpp"
# endif
# include "../types/byte_t.h"
# include "../types/pixmap_t.h"
# include "../types/err_t.h"
# include "../system/errno.h"
namespace mdl {
namespace firefly {
namespace graphics {
types::err_t cpu_draw_pixmap(uint_t __xfs, uint_t __yfs, types::pixmap_t __pixbuff, uint_t __pb_xlen, uint_t __pb_ylen, types::pixmap_t __pixmap, uint_t __pm_xlen, uint_t __pm_ylen);

# if defined(__USING_OPENCL) || defined(USING_OPENCL)
types::err_t draw_pixmap(uint_t __xfs, uint_t __yfs, types::pixmap_t __pixbuff, uint_t __pb_xlen, uint_t __pb_ylen, types::pixmap_t __pixmap, uint_t __pm_xlen, uint_t __pm_ylen, opencl *__opencl);
# elif defined(__USING_CUDA) || defined(USING_CUDA)
types::err_t gpu_draw_pixmap(uint_t __xfs, uint_t __yfs, types::pixmap_t __pixbuff, uint_t __pb_xlen, uint_t __pb_ylen, uint_t __pm_xfs, types::pixmap_t __pixmap, uint_t __pm_xlen, uint_t __pm_ylen, uint_t __rpm_xlen, boost::uint16_t __angle = 0);
# endif
types::err_t draw_pixmap(uint_t __xfs, uint_t __yfs, types::pixmap_t __pixbuff, uint_t __pb_xlen, uint_t __pb_ylen, types::pixmap_t __pixmap, uint_t __pm_xlen, uint_t __pm_ylen, boost::uint16_t __angle = 0);
}
}
}

# endif /*__graphics__draw__pixmap__hpp*/
