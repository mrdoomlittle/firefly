# include "uni_manager.hpp"
mdl::firefly::types::err_t mdl::firefly::uni_manager::init(uint_t __xa_split, uint_t __ya_split, uint_t __za_split) {
	uint_t cnk_xa_len = this-> xaxis_len / __xa_split;
	if (cnk_xa_len % 1) {
		fprintf(stderr, "can't use %d for the chunk xaxis split.\n", __xa_split);
		return FFLY_FAILURE;
	}

	uint_t cnk_ya_len = this-> yaxis_len / __ya_split;
	if (cnk_ya_len % 1) {
		fprintf(stderr, "can't use %d for the chunk yaxis split.\n", __ya_split);
		return FFLY_FAILURE;
	}

	uint_t cnk_za_len = this-> zaxis_len / __za_split;
	if (cnk_za_len % 1) {
		fprintf(stderr, "can't use %d for the chunk zaxis split.\n", __za_split);
		return FFLY_FAILURE;
	}

	this-> cnk_xc = __xa_split;
	this-> cnk_yc = __ya_split;
	this-> cnk_zc = __za_split;
	uint_t cnk_c = cnk_xa_len * cnk_ya_len * cnk_za_len;

	if ((this-> cnk_id_indx = (uint_t **)memory::mem_alloc(cnk_c * sizeof(uint_t *))) == NULL) {
		fprintf(stderr, "uni_manager: failed to alloc memory for 'chunk_id_index', errno: %d\n", errno);
		return FFLY_FAILURE;
	}

	if ((this-> _1d_uni_pm = (types::_1d_pm_t *)memory::mem_alloc(cnk_c * sizeof(types::_1d_pm_t *))) == NULL) {
		fprintf(stderr, "uni_manager: failed to alloc memory for '_1d_uni_pm', errno: %d\n", errno);
		memory::mem_free(this-> cnk_id_indx);
		return FFLY_FAILURE;
	}

	if ((this-> _3d_uni_pm = (types::_3d_pm_t *)memory::mem_alloc(cnk_c * sizeof(types::_3d_pm_t *))) == NULL) {
		fprintf(stderr, "uni_manager: failed to alloc memory for '_3d_uni_pm', errno: %d\n", errno);
		memory::mem_free(this-> cnk_id_indx);
		memory::mem_free(this-> _1d_uni_pm);
		return FFLY_FAILURE;
	}

	this-> uni_particles = (types::uni_par_t **)memory::mem_alloc(cnk_c * sizeof(types::uni_par_t *));
	if (this-> uni_particles == NULL) {
		fprintf(stderr, "uni_manager: failed to alloc memory for 'uni_particles', errno: %d\n", errno);
		memory::mem_free(this-> _1d_uni_pm);
		memory::mem_free(this-> _3d_uni_pm);
		memory::mem_free(this-> cnk_id_indx);
		return FFLY_FAILURE;
	}

	printf("uni_manager: chunk size has been set as %dx%dx%d\n", cnk_xa_len, cnk_ya_len, cnk_za_len);
	static chunk_manager _chunk_manager(cnk_xa_len, cnk_ya_len, cnk_za_len);
	this-> _chunk_manager = &_chunk_manager;
	uint_t cnk_point = 0;
	for (uint_t za{}; za != this-> zaxis_len; za += cnk_za_len) {
		for (uint_t ya{}; ya != this-> yaxis_len; ya += cnk_ya_len) {
			for (uint_t xa{}; xa != this-> xaxis_len; xa += cnk_xa_len) {
				types::id_t cnk_id = nullptr;

				_chunk_manager.add_cnk(cnk_id, xa, ya, za);

				types::chunk_data_t& cnk_data = _chunk_manager.cnk_data(cnk_id);

				this-> _1d_uni_pm[cnk_point] = cnk_data._1d_pm;
				this-> _3d_uni_pm[cnk_point] = cnk_data._3d_pm;
				this-> uni_particles[cnk_point] = cnk_data.particles;
				this-> cnk_id_indx[cnk_point] = cnk_id;
				cnk_point++;
			}
		}
	}
	printf("uni_manager: there are %d chunk in this uni.\n", cnk_point);
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::uni_manager::draw_cnk(uint_t __xfs, uint_t __yfs, uint_t __zfs, types::id_t __cnk_id, types::pixmap_t __pixbuff, uint_t __xaxis_len, uint_t __yaxis_len) {
	types::pixmap_t cnk_pixmap;
	if (!(cnk_pixmap = this-> _chunk_manager-> cnk_data(__cnk_id)._1d_pm)) return FFLY_FAILURE;

	types::err_t any_err;
	if ((any_err = graphics::draw_pixmap(__xfs, __yfs, __pixbuff, __xaxis_len, __yaxis_len, cnk_pixmap, this-> _chunk_manager-> get_cnk_xlen(), this-> _chunk_manager-> get_cnk_ylen())) != FFLY_SUCCESS) return any_err;
	return FFLY_SUCCESS;
}

mdl::firefly::types::_1d_pm_t mdl::firefly::uni_manager::get_1d_cnk_pm(uint_t __xa, uint_t __ya, uint_t __za) {
	uint_t cnk_xa, cnk_ya, cnk_za;
	this-> get_cnk_coords(__xa, __ya, __za, cnk_xa, cnk_ya, cnk_za);
	return this-> _1d_cnk_pm(cnk_xa, cnk_ya, cnk_za);
}

mdl::firefly::types::_3d_pm_t mdl::firefly::uni_manager::get_3d_cnk_pm(uint_t __xa, uint_t __ya, uint_t __za) {
	uint_t cnk_xa, cnk_ya, cnk_za;
	this-> get_cnk_coords(__xa, __ya, __za, cnk_xa, cnk_ya, cnk_za);
	return this-> _3d_cnk_pm(cnk_xa, cnk_ya, cnk_za);
}

void mdl::firefly::uni_manager::get_cnk_coords(uint_t __xa, uint_t __ya, uint_t __za, uint_t& __cnk_xa, uint_t& __cnk_ya, uint_t& __cnk_za) {
	__cnk_xa = floor(__xa / this-> _chunk_manager-> get_cnk_xlen());
	__cnk_ya = floor(__ya / this-> _chunk_manager-> get_cnk_ylen());
	__cnk_za = floor(__za / this-> _chunk_manager-> get_cnk_zlen());
}

void mdl::firefly::uni_manager::set_par_colour(uint_t __xa, uint_t __ya, uint_t __za, u8_t __r, u8_t __g, u8_t __b, u8_t __a) {
	uint_t xa = __xa * UNI_PAR_XLEN, ya = __ya * UNI_PAR_YLEN, za = __za * UNI_PAR_ZLEN;

	types::_3d_pm_t pm[8] = {
		this-> get_3d_cnk_pm(xa, ya, za),
		this-> get_3d_cnk_pm(xa + UNI_PAR_XLEN, ya, za),
		this-> get_3d_cnk_pm(xa, ya + UNI_PAR_YLEN, za),
		this-> get_3d_cnk_pm(xa, ya, za + UNI_PAR_ZLEN),

		this-> get_3d_cnk_pm(xa + UNI_PAR_XLEN, ya + UNI_PAR_YLEN, za),

		this-> get_3d_cnk_pm(xa + UNI_PAR_XLEN, ya, za + UNI_PAR_ZLEN),

		this-> get_3d_cnk_pm(xa, ya + UNI_PAR_YLEN, za + UNI_PAR_ZLEN),
		this-> get_3d_cnk_pm(xa + UNI_PAR_XLEN, ya + UNI_PAR_YLEN, za + UNI_PAR_ZLEN)
	};

	uint_t xe_pad = 0, ye_pad = 0, ze_pad = 0;
	if (xa + UNI_PAR_XLEN > this-> _chunk_manager-> get_cnk_xlen())
		xe_pad = (xa + UNI_PAR_XLEN) - this-> _chunk_manager-> get_cnk_xlen();
	if (ya + UNI_PAR_YLEN > this-> _chunk_manager-> get_cnk_ylen())
		ye_pad = (ya + UNI_PAR_YLEN) - this-> _chunk_manager-> get_cnk_ylen();
	if (za + UNI_PAR_ZLEN > this-> _chunk_manager-> get_cnk_zlen())
		ze_pad = (za + UNI_PAR_ZLEN) - this-> _chunk_manager-> get_cnk_zlen();

	for (uint_t _za{xa}; _za != (za + UNI_PAR_ZLEN) - ze_pad; _za ++) {
		for (uint_t _ya{ya}; _ya != (ya + UNI_PAR_YLEN) - ye_pad; _ya ++) {
			for (uint_t _xa{xa}; _xa != (xa + UNI_PAR_XLEN) - xe_pad; _xa ++)
				graphics::rgba_set_pm_pix(__r, __g, __b, __a, pm[0], _xa, _ya, _za);

			if (pm[1] != pm[0])
				for (uint_t _xa{}; _xa != xe_pad; _xa ++)
					graphics::rgba_set_pm_pix(__r, __g, __b, __a, pm[1], _xa, _ya, _za);
		}
		if (pm[2] != pm[0]) {
			for (uint_t _ya{}; _ya != ye_pad; _ya ++) {
				for (uint_t _xa{xa}; _xa != (xa + UNI_PAR_XLEN) - xe_pad; _xa ++)
					graphics::rgba_set_pm_pix(__r, __g, __b, __a, pm[2], _xa, _ya, _za);

				if (pm[4] != pm[2])
					for (uint_t _xa{}; _xa != xe_pad; _xa ++)
						graphics::rgba_set_pm_pix(__r, __g, __b, __a, pm[4], _xa, _ya, _za);
			}
		}
	}

	if (pm[3] != pm[0]) {
		for (uint_t _za{}; _za != ze_pad; _za ++) {
			for (uint_t _ya{ya}; _ya != (ya + UNI_PAR_YLEN) - ye_pad; _ya ++) {
				for (uint_t _xa{xa}; _xa != (xa + UNI_PAR_XLEN) - xe_pad; _xa ++)
					graphics::rgba_set_pm_pix(__r, __g, __b, __a, pm[3], _xa, _ya, _za);

				if (pm[5] != pm[3])
					for (uint_t _xa{}; _xa != xe_pad; _xa ++)
						graphics::rgba_set_pm_pix(__r, __g, __b, __a, pm[5], _xa, _ya, _za);
			}


			if (pm[6] != pm[3]) {
				for (uint_t _ya{}; _ya != ye_pad; _ya ++) {
					for (uint_t _xa{xa}; _xa != (xa + UNI_PAR_XLEN) - xe_pad; _xa ++)
						graphics::rgba_set_pm_pix(__r, __g, __b, __a, pm[3], _xa, _ya, _za);

					if (pm[7] != pm[6])
						for (uint_t _xa{}; _xa != xe_pad; _xa ++)
							graphics::rgba_set_pm_pix(__r, __g, __b, __a, pm[7], _xa, _ya, _za);
				}
			}
		}
	}
}

void mdl::firefly::uni_manager::get_par_colour(uint_t __xa, uint_t __ya, uint_t __za, u8_t& __r, u8_t& __g, u8_t& __b, u8_t& __a) {
	types::_3d_pm_t pm = this-> get_3d_cnk_pm(__xa, __ya, __za);
	__r = pm[__za][__ya][__xa];
	__g = pm[__za][__ya][__xa + 1];
	__b = pm[__za][__ya][__xa + 2];
	__a = pm[__za][__ya][__xa + 3];
}
