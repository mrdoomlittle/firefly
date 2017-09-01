# include "../types/pixmap_t.h"
# include "../maths/rotate_point.hpp"
# include "../memory/alloc_pixmap.hpp"
# include "../memory/mem_free.h"
# include <eint_t.hpp>
# include "../system/matrix.hpp"
namespace mdl {
namespace firefly {
void rotate_pixmap(types::pixmap_t& __pm, uint_t& __xa_len, uint_t& __ya_len) {
	types::pixmap_t pix_buff = memory::alloc_pixmap(__xa_len*3, __ya_len*3);
	memset(pix_buff, 0, (__xa_len*3)*(__ya_len*3)*4);

	for (uint_t ya{}; ya != __ya_len; ya++) {
		for (uint_t xa{}; xa != __xa_len; xa++) {
			int_t _xa = xa, _ya = ya;
			maths::rotate_point(_xa, _ya, 45, 1);
			if (_xa < 0) _xa = __xa_len-(-_xa); else _xa += __xa_len;
			if (_ya < 0) _ya = __ya_len-(-_ya); else _ya += __ya_len;

			uint_t pix_loc = (xa+(ya*__xa_len))*4;
			uint_t n_pix_loc = (_xa+(_ya*(__xa_len*3)))*4;

			pix_buff[n_pix_loc] = __pm[pix_loc];
			pix_buff[n_pix_loc+1] = __pm[pix_loc+1];
			pix_buff[n_pix_loc+2] = __pm[pix_loc+2];
			pix_buff[n_pix_loc+3] = __pm[pix_loc+3];
		}
	}

	__xa_len *= 3;
	__ya_len *= 3;
	__ffly_mem_free(__pm);
	__pm = pix_buff;
}

}
}
