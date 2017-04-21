# ifndef __crop__pixmap__hpp
# define __crop__pixmap__hpp
# include "../types/err_t.h"
# include "../types/pixmap_t.h"
# include <eint_t.hpp>
# include <cstdio>
# include "../types/byte_t.h"
# include "../system/errno.h"
namespace mdl {
namespace firefly {
namespace graphics {
/*
	types::pixmap_t pixbuff; <- the place where the crop will be placed
	types::pixmap_t pixmap; <- the image that we are croping from
*/
# if defined(__USING_OPENCL) || defined(USING_OPENCL)
# elif defined(__USING_CUDA) || defined(USING_CUDA)
types::err_t gpu_crop_2d_pm(uint_t __xfs, uint_t __yfs, types::_2d_pm_t __pixbuff, uint_t __pb_xlen, uint_t __pb_ylen, uint_t __rpb_xlen,
	types::_2d_pm_t __pixmap, uint_t __pm_xlen, uint_t __pm_ylen, uint_t __pm_rxlen, u8_t __chn_c);

types::err_t gpu_crop_pixmap(uint_t __xfs, uint_t __yfs, types::pixmap_t __pixbuff, uint_t __pb_xlen, uint_t __pb_ylen, types::pixmap_t __pixmap, uint_t __pm_xlen, uint_t __pm_ylen, u8_t __chn_c);
# endif
types::err_t static crop_2d_pm(uint_t __xfs, uint_t __yfs, types::_2d_pm_t __pixbuff, uint_t __pb_xlen, uint_t __pb_ylen, uint_t __pb_rxlen,
	types::_2d_pm_t __pixmap, uint_t __pm_xlen, uint_t __pm_ylen, uint_t __pm_rxlen, u8_t __chn_c) {
	return gpu_crop_2d_pm(__xfs, __yfs, __pixbuff, __pb_xlen, __pb_ylen, __pb_rxlen, __pixmap, __pm_xlen, __pm_ylen, __pm_rxlen, __chn_c);
}

types::err_t static crop_pixmap(uint_t __xfs, uint_t __yfs, types::pixmap_t __pixbuff, uint_t __pb_xlen, uint_t __pb_ylen, types::pixmap_t __pixmap, uint_t __pm_xlen, uint_t __pm_ylen, u8_t __chn_c) {
	return gpu_crop_pixmap(__xfs, __yfs, __pixbuff, __pb_xlen, __pb_ylen, __pixmap, __pm_xlen, __pm_ylen, __chn_c);
}
}
}
}

# endif /*__crop__pixmap__hpp*/
