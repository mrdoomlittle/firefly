# include "scale_pixmap.hpp"

mdl::firefly::types::err_t mdl::firefly::data::cpu_scale_pixmap(types::pixmap_t& __pixmap, uint_t& __xaxis_len, uint_t& __yaxis_len, int_t __xscale, int_t __yscale, boost::int8_t __type) {
	if (__xscale < 1 && __yscale < 1) return FFLY_NOP;
	uint_t xaxis_len = __xaxis_len * __xscale;
	uint_t yaxis_len = __yaxis_len * __yscale;

	types::pixmap_t pixmap;
	if ((pixmap = memory::alloc_pixmap(xaxis_len, yaxis_len)) == NULL) {
		return FFLY_FAILURE;
	}

	if (__type == CUBIC_SCALE) {
		uint_t __xaxis = 0, __yaxis = 0;
		uint_t xaxis = 0, yaxis = 0;
		while(yaxis != __yaxis_len) {
			__xaxis = 0;
			xaxis = 0;
			while(xaxis != __xaxis_len) {
				uint_t pix_point = (xaxis + (yaxis * __xaxis_len)) * 4;

				for (uint_t _yaxis = __yaxis; _yaxis != __yaxis + __yscale; _yaxis ++) {
					for (uint_t _xaxis = __xaxis; _xaxis != __xaxis + __xscale; _xaxis ++) {
						uint_t _pix_point = (_xaxis + (_yaxis * xaxis_len)) * 4;

						pixmap[_pix_point] = __pixmap[pix_point];
						pixmap[_pix_point + 1] = __pixmap[pix_point + 1];
						pixmap[_pix_point + 2] = __pixmap[pix_point + 2];
						pixmap[_pix_point + 3] = __pixmap[pix_point + 3];
					}
				}
				__xaxis += __xscale;
				xaxis ++;
			}
			__yaxis += __yscale;
			yaxis ++;
		}

		goto finish;
	}

	if (__type == LINEAR_SCALE) {
		return FFLY_NOP;
		uint_t __xaxis = 0, __yaxis = 0;
		uint_t xaxis = 0, yaxis = 0;

		while(yaxis != __yaxis_len) {
			__xaxis = 0;
			xaxis = 0;
			while(xaxis != __xaxis_len) {
				uint_t pix_point = (xaxis + (yaxis * __xaxis_len)) * 4;

				uint_t y_fs = 0;
				for (uint_t _yaxis = __yaxis; _yaxis != __yaxis + __yscale; _yaxis ++) {
					uint_t x_fs = 0;
					for (uint_t _xaxis = __xaxis; _xaxis != __xaxis + __xscale; _xaxis ++) {
						uint_t _pix_point = (_xaxis + (_yaxis * xaxis_len)) * 4;

						if (xaxis == __xaxis_len - 1){
							pixmap[_pix_point] = __pixmap[pix_point];
							pixmap[_pix_point + 1] = __pixmap[pix_point + 1];
							pixmap[_pix_point + 2] = __pixmap[pix_point + 2];
							pixmap[_pix_point + 3] = __pixmap[pix_point + 3];
							continue;
						}

						pixmap[_pix_point] = pixmap[_pix_point+1] = pixmap[_pix_point +2] = 0;

						float _1c, _2c, _3c, _4c, _5c, _6c;

						_1c = ((float)__pixmap[pix_point] / (float)__xscale) * ((float)__xscale - (float)x_fs);
						_2c = ((float)__pixmap[pix_point + 4] / (float)__xscale) * (float)x_fs;
						pixmap[_pix_point] = (types::byte_t)((float)__pixmap[pix_point] * (_1c + _2c) / 255);

						_1c = ((float)__pixmap[pix_point + 1] / (float)__xscale) * ((float)__xscale - (float)x_fs);
						_2c = ((float)__pixmap[pix_point + 4 + 1] / (float)__xscale) * (float)x_fs;
						pixmap[_pix_point + 1] = (types::byte_t)((float)__pixmap[pix_point + 1] * (_1c + _2c) / 255);

						_1c = ((float)__pixmap[pix_point + 2] / (float)__xscale) * ((float)__xscale - (float)x_fs);
						_2c = ((float)__pixmap[pix_point + 4 + 2] / (float)__xscale) * (float)x_fs;
						pixmap[_pix_point + 2] = (types::byte_t)((float)__pixmap[pix_point + 2] * (_1c + _2c) / 255);

						pixmap[_pix_point + 3] = __pixmap[pix_point + 3];
						x_fs ++;
					}
					y_fs ++;
				}
				__xaxis += __xscale;
				xaxis ++;
			}
			__yaxis += __yscale;
			yaxis ++;
		}
		goto finish;
	}

	memory::mem_free(pixmap);
	return FFLY_FAILURE;

	finish:
	memory::mem_free(__pixmap);
	__pixmap = pixmap;

	__xaxis_len = xaxis_len;
	__yaxis_len = yaxis_len;

	return FFLY_SUCCESS;
}
