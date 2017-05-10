# include "chunk_keeper.hpp"
mdl::firefly::types::err_t mdl::firefly::chunk_keeper::create_cnk(types::id_t& __cnk_id, bool __overwrite) {
	if (__cnk_id != nullptr && !__overwrite) return FFLY_NOP;

	if (this-> cnk_c == 0) {
		if ((this-> cnk_info = (types::chunk_info_t *)memory::mem_alloc(sizeof(types::chunk_info_t))) == NULL) {
			fprintf(stderr, "chunk_keeper: failed to alloc memory for 'chunk_info', errno: %d\n", errno);
			return FFLY_FAILURE;
		}

		if ((this-> cnk_data = (types::chunk_data_t *)memory::mem_alloc(sizeof(types::chunk_data_t))) == NULL) {
			fprintf(stderr, "chunk_keeper: failed to alloc memory for 'chunk_data', errno: %d\n", errno);
			return FFLY_FAILURE;
		}
	} else {
		types::chunk_info_t *cnk_info = NULL;
		if ((cnk_info = (types::chunk_info_t *)memory::mem_realloc(this-> cnk_info, (this-> cnk_c + 1) * sizeof(types::chunk_info_t))) == NULL) {
			fprintf(stderr, "chunk_keeper: failed to realloc memory for 'chunk_info', errno: %d\n", errno);
			return FFLY_FAILURE;
		} else
			this-> cnk_info = cnk_info;

		types::chunk_data_t *cnk_data = NULL;
		if ((cnk_data = (types::chunk_data_t *)memory::mem_realloc(this-> cnk_data, (this-> cnk_c + 1) * sizeof(types::chunk_data_t))) == NULL) {
			fprintf(stderr, "chunk_keeper: failed to realloc memory for 'chunk_data', errno: %d\n", errno);
			cnk_info = NULL;
			if ((cnk_info = (types::chunk_info_t *)memory::mem_realloc(this-> cnk_info, (this-> cnk_c - 1) * sizeof(types::chunk_info_t))) == NULL)
				return FFLY_FAILURE;
			else
				this-> cnk_info = cnk_info;
		} else
			this-> cnk_data = cnk_data;
	}

	types::chunk_info_t *cnk_info = nullptr;
	types::chunk_data_t *cnk_data = nullptr;
	uint_t rxaxis_len, ryaxis_len, rzaixs_len;

	if (!this-> unused_ids.empty()) {
		std::set<uint_t *>::iterator itor = this-> unused_ids.end();
		--itor;

		__cnk_id = *itor;
		this-> unused_ids.erase(itor);
	} else {
		this-> cnk_info[this-> cnk_c].id, __cnk_id = (types::id_t)memory::mem_alloc(sizeof(types::__id_t));
		if (__cnk_id == NULL) {
			fprintf(stderr, "chunk_keeper: failed to alloc memory for 'chunk_id', errno: %d\n", errno);
			goto mem_clean;
		} else
			*this-> cnk_info[this-> cnk_c].id, *__cnk_id = this-> cnk_c;
	}

	cnk_info = &this-> cnk_info[*__cnk_id];
	cnk_data = &this-> cnk_data[*__cnk_id];

	if ((cnk_data-> particles = (types::uni_par_t *)memory::mem_alloc((this-> xaxis_len * this-> yaxis_len * this-> zaxis_len) * sizeof(types::uni_par_t))) == NULL) {
		fprintf(stderr, "chunk_keeper: failed to alloc memory for chunk particles, errno: %d\n", errno);
		goto mem_clean;
	}

	rxaxis_len = this-> xaxis_len * UNI_PAR_XLEN;
	ryaxis_len = this-> yaxis_len * UNI_PAR_YLEN;
	rzaixs_len = this-> zaxis_len * UNI_PAR_ZLEN;
	if ((cnk_data-> _1d_pm = memory::alloc_pixmap(rxaxis_len, ryaxis_len, rzaixs_len, 4)) == NULL) {
		fprintf(stderr, "chunk_keeper: failed to alloc pixmap for chunk, errno: %d\n", errno);
		goto mem_clean;
	}

	cnk_data-> _3d_pm = memory::make_3d_pm(cnk_data-> _1d_pm, rxaxis_len, ryaxis_len, rzaixs_len, 4);

	// ignore
	if (this-> cnk_c == 1)
		memset(cnk_data-> _1d_pm, 244, (rxaxis_len * ryaxis_len * rzaixs_len) * 4);
	else
		memset(cnk_data-> _1d_pm, 100, (rxaxis_len * ryaxis_len * rzaixs_len) * 4);

	for (int y{}; y != 80; y ++) {memset(cnk_data-> _3d_pm[0][y], 244, 80*4);}
	//

	this-> cnk_c ++;

	return FFLY_SUCCESS;
	mem_clean:

	if (this-> cnk_c == 0) {
		memory::mem_free(this-> cnk_info);
		memory::mem_free(this-> cnk_data);
	} else {
		this-> cnk_info = (types::chunk_info_t *)memory::mem_realloc(this-> cnk_info, (this-> cnk_c - 1) * sizeof(types::chunk_info_t));
		this-> cnk_data = (types::chunk_data_t *)memory::mem_realloc(this-> cnk_data, (this-> cnk_c - 1) * sizeof(types::chunk_data_t));
	}

	if (__cnk_id != NULL) memory::mem_free(__cnk_id);
	if (this-> cnk_data-> particles != NULL) memory::mem_free(cnk_data-> particles);
	if (this-> cnk_data-> _1d_pm != NULL) memory::mem_free(cnk_data-> _1d_pm);
	return FFLY_FAILURE;
}

mdl::firefly::types::err_t mdl::firefly::chunk_keeper::del_cnk(types::id_t __cnk_id, bool __hard) {
	if (__cnk_id == nullptr) return FFLY_NOP;

	types::chunk_info_t& cnk_info = this-> cnk_info[*__cnk_id];
	types::chunk_data_t& cnk_data = this-> cnk_data[*__cnk_id];

	if (__hard) memory::mem_free(cnk_info.id);

	memory::mem_free(cnk_data.particles);
	memory::mem_free(cnk_data._1d_pm);

	if (__hard) {
		uint_t end_id = this-> cnk_c;
		this-> cnk_c --;

		if (*__cnk_id == end_id) return FFLY_SUCCESS;

		cnk_info = this-> cnk_info[end_id];
		cnk_data = this-> cnk_data[end_id];

		this-> cnk_info = (types::chunk_info_t *)realloc(this-> cnk_info, this-> cnk_c * sizeof(types::chunk_info_t));
		if (this-> cnk_info == NULL) {
			fprintf(stderr, "failed to relocate memory for 'chunk_info', errno: %d\n", errno);
			return FFLY_FAILURE;
		}

		this-> cnk_data = (types::chunk_data_t *)realloc(this-> cnk_data, this-> cnk_c * sizeof(types::chunk_data_t));
		if (this-> cnk_data == NULL) {
			fprintf(stderr, "failed to relocate memory for 'chunk_data', errno: %d\n", errno);
			return FFLY_FAILURE;
		}
	} else
		this-> unused_ids.insert(__cnk_id);
	return FFLY_SUCCESS;
}
