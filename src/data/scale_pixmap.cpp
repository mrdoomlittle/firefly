# include "scale_pixmap.hpp"

mdl::firefly::types::err_t mdl::firefly::data::cpu_scale_pixmap(types::pixmap_t& __pixmap, uint_t& __xa_len, uint_t& __ya_len, int_t __xscale, int_t __yscale, boost::int8_t __type) {
	if (__xscale < 1 && __yscale < 1) return FFLY_NOP;
	uint_t xa_len = __xa_len*__xscale;
	uint_t ya_len = __ya_len*__yscale;

	types::pixmap_t pixmap;
	if ((pixmap = memory::alloc_pixmap(xa_len, ya_len)) == NULL) {
		return FFLY_FAILURE;
	}

	if (__type == CUBIC_SCALE) {
		uint_t __xa = 0, __ya = 0;
		uint_t xa = 0, ya = 0;
		while(ya != __ya_len) {
			__xa = 0;
			xa = 0;
			while(xa != __xa_len) {
				uint_t pix_point = (xa + (ya * __xa_len)) * 4;

				for (uint_t _ya = __ya; _ya != __ya + __yscale; _ya ++) {
					for (uint_t _xa = __xa; _xa != __xa + __xscale; _xa ++) {
						uint_t _pix_point = (_xa + (_ya * xa_len)) * 4;

						pixmap[_pix_point] = __pixmap[pix_point];
						pixmap[_pix_point + 1] = __pixmap[pix_point + 1];
						pixmap[_pix_point + 2] = __pixmap[pix_point + 2];
						pixmap[_pix_point + 3] = __pixmap[pix_point + 3];
					}
				}
				__xa += __xscale;
				xa ++;
			}
			__ya += __yscale;
			ya ++;
		}

		goto finish;
	}

	if (__type == LINEAR_SCALE) {
		uint_t __xa{}, __ya{};
		uint_t xa{}, ya{};
		while(ya != __ya_len) {
			__xa = 0;
			xa = 0;
			while(xa != __xa_len) {
				uint_t pix_loc = (xa+(ya*__xa_len))*4;
				float tl_eg_r, tl_eg_g, tl_eg_b, tl_eg_a;
				float tr_eg_r, tr_eg_g, tr_eg_b, tr_eg_a;
				float bl_eg_r, bl_eg_g, bl_eg_b, bl_eg_a;
				float br_eg_r, br_eg_g, br_eg_b, br_eg_a;

				tl_eg_r = (float)__pixmap[pix_loc];
				tl_eg_g = (float)__pixmap[pix_loc+1];
				tl_eg_b = (float)__pixmap[pix_loc+2];
				tl_eg_a = (float)__pixmap[pix_loc+3];

				tr_eg_r = (float)__pixmap[pix_loc+4];
				tr_eg_g = (float)__pixmap[pix_loc+4+1];
				tr_eg_b = (float)__pixmap[pix_loc+4+2];
				tr_eg_a = (float)__pixmap[pix_loc+4+3];

				bl_eg_r = (float)__pixmap[pix_loc+(__xa_len*4)];
				bl_eg_g = (float)__pixmap[pix_loc+(__xa_len*4)+1];
				bl_eg_b = (float)__pixmap[pix_loc+(__xa_len*4)+2];
				bl_eg_a = (float)__pixmap[pix_loc+(__xa_len*4)+3];

				br_eg_r = (float)__pixmap[pix_loc+(__xa_len*4)+4];
				br_eg_g = (float)__pixmap[pix_loc+(__xa_len*4)+4+1];
				br_eg_b = (float)__pixmap[pix_loc+(__xa_len*4)+4+2];
				br_eg_a = (float)__pixmap[pix_loc+(__xa_len*4)+4+3];

				for (uint_t _ya{__ya}; _ya != __ya+__yscale; _ya++) {
					for (uint_t _xa{__xa}; _xa != __xa+__xscale; _xa++) {
						uint_t _pix_loc = (_xa+(_ya*xa_len))*4;
						float xa_dist = (float)_xa-(float)__xa;
						float ya_dist = (float)_ya-(float)__ya;

						float _tr = (((((float)__xscale-(float)xa_dist)*(tl_eg_r/(float)__xscale))*tl_eg_r)+(((float)xa_dist*(tr_eg_r/(float)__xscale))*tr_eg_r))/255.0;
						float _br = (((((float)__xscale-(float)xa_dist)*(bl_eg_r/(float)__xscale))*bl_eg_r)+(((float)xa_dist*(br_eg_r/(float)__xscale))*br_eg_r))/255.0;

						float _lr = (((((float)__yscale-(float)ya_dist)*(tl_eg_r/(float)__yscale))*tl_eg_r)+(((float)ya_dist*(bl_eg_r/(float)__yscale))*bl_eg_r))/255.0;
						float _rr = (((((float)__yscale-(float)ya_dist)*(tr_eg_r/(float)__yscale))*tr_eg_r)+(((float)ya_dist*(br_eg_r/(float)__yscale))*br_eg_r))/255.0;

						float r = ((((float)__xscale-(float)xa_dist)*(_lr/(float)__xscale))*_lr)+(((float)xa_dist*(_rr/(float)__xscale))*_rr)+((((float)__yscale-(float)ya_dist)*(_tr/(float)__yscale))*_tr)+(((float)ya_dist*(_br/(float)__yscale))*_br);
						r /= 2;

						float _tg = (((((float)__xscale-(float)xa_dist)*(tl_eg_g/(float)__xscale))*tl_eg_g)+(((float)xa_dist*(tr_eg_g/(float)__xscale))*tr_eg_g))/255.0;
						float _bg = (((((float)__xscale-(float)xa_dist)*(bl_eg_g/(float)__xscale))*bl_eg_g)+(((float)xa_dist*(br_eg_g/(float)__xscale))*br_eg_g))/255.0;

						float _lg = (((((float)__yscale-(float)ya_dist)*(tl_eg_g/(float)__yscale))*tl_eg_g)+(((float)ya_dist*(bl_eg_g/(float)__yscale))*bl_eg_g))/255.0;
						float _rg = (((((float)__yscale-(float)ya_dist)*(tr_eg_g/(float)__yscale))*tr_eg_g)+(((float)ya_dist*(br_eg_g/(float)__yscale))*br_eg_g))/255.0;

						float g = ((((float)__xscale-(float)xa_dist)*(_lg/(float)__xscale))*_lg)+(((float)xa_dist*(_rg/(float)__xscale))*_rg)+((((float)__yscale-(float)ya_dist)*(_tg/(float)__yscale))*_tg)+(((float)ya_dist*(_bg/(float)__yscale))*_bg);
						g /= 2;

						float _tb = (((((float)__xscale-(float)xa_dist)*(tl_eg_b/(float)__xscale))*tl_eg_b)+(((float)xa_dist*(tr_eg_b/(float)__xscale))*tr_eg_b))/255.0;
						float _bb = (((((float)__xscale-(float)xa_dist)*(bl_eg_b/(float)__xscale))*bl_eg_b)+(((float)xa_dist*(br_eg_b/(float)__xscale))*br_eg_b))/255.0;

						float _lb = (((((float)__yscale-(float)ya_dist)*(tl_eg_b/(float)__yscale))*tl_eg_b)+(((float)ya_dist*(bl_eg_b/(float)__yscale))*bl_eg_b))/255.0;
						float _rb = (((((float)__yscale-(float)ya_dist)*(tr_eg_b/(float)__yscale))*tr_eg_b)+(((float)ya_dist*(br_eg_b/(float)__yscale))*br_eg_b))/255.0;

						float b = ((((float)__xscale-(float)xa_dist)*(_lb/(float)__xscale))*_lb)+(((float)xa_dist*(_rb/(float)__xscale))*_rb)+((((float)__yscale-(float)ya_dist)*(_tb/(float)__yscale))*_tb)+(((float)ya_dist*(_bb/(float)__yscale))*_bb);
						b /= 2;


						pixmap[_pix_loc] = (types::byte_t)(r/255.0);
						pixmap[_pix_loc+1] = (types::byte_t)(g/255.0);
						pixmap[_pix_loc+2] = (types::byte_t)(b/255.0);
						pixmap[_pix_loc+3] = 255;

						//printf("r: %u,	g: %u,	b: %u,	a: %u\n", pixmap[_pix_loc], pixmap[_pix_loc+1], pixmap[_pix_loc+2], pixmap[_pix_loc+3]);
					}
				}
				__xa += __xscale;
				xa++;
			}
			__ya += __yscale;
			ya++;
		}
		goto finish;
	}

	memory::mem_free(pixmap);
	return FFLY_FAILURE;

	finish:
	memory::mem_free(__pixmap);
	__pixmap = pixmap;

	__xa_len = xa_len;
	__ya_len = ya_len;

	return FFLY_SUCCESS;
}
