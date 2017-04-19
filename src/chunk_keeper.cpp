# include "chunk_keeper.hpp"
boost::int8_t mdl::firefly::chunk_keeper::create_chunk(uint_t*& __chunk_id, bool __overwrite) {
	if (__chunk_id != nullptr && !__overwrite) return FFLY_NOP;

	if (this-> chunk_count == 0) {
		if ((this-> chunk_info = (types::chunk_info_t *)memory::mem_alloc(sizeof(types::chunk_info_t))) == NULL) {
			fprintf(stderr, "chunk_keeper: failed to alloc memory for 'chunk_info', errno: %d\n", errno);
			return FFLY_FAILURE;
		}

		if ((this-> chunk_data = (types::chunk_data_t *)memory::mem_alloc(sizeof(types::chunk_data_t))) == NULL) {
			fprintf(stderr, "chunk_keeper: failed to alloc memory for 'chunk_data', errno: %d\n", errno);
			return FFLY_FAILURE;
		}
	} else {
		types::chunk_info_t *chunk_info = NULL;
		if ((chunk_info = (types::chunk_info_t *)memory::mem_realloc(this-> chunk_info, (this-> chunk_count + 1) * sizeof(types::chunk_info_t))) == NULL) {
			fprintf(stderr, "chunk_keeper: failed to realloc memory for 'chunk_info', errno: %d\n", errno);
			return FFLY_FAILURE;
		} else
			this-> chunk_info = chunk_info;

		types::chunk_data_t *chunk_data = NULL;
		if ((chunk_data = (types::chunk_data_t *)memory::mem_realloc(this-> chunk_data, (this-> chunk_count + 1) * sizeof(types::chunk_data_t))) == NULL) {
			fprintf(stderr, "chunk_keeper: failed to realloc memory for 'chunk_data', errno: %d\n", errno);
			chunk_info = NULL;
			if ((chunk_info = (types::chunk_info_t *)memory::mem_realloc(this-> chunk_info, (this-> chunk_count - 1) * sizeof(types::chunk_info_t))) == NULL)
				return FFLY_FAILURE;
			else
				this-> chunk_info = chunk_info;
		} else
			this-> chunk_data = chunk_data;
	}

	types::chunk_info_t *chunk_info = nullptr;
	types::chunk_data_t *chunk_data = nullptr;
	uint_t rxaxis_len, ryaxis_len, rzaixs_len;

	if (!this-> unused_ids.empty()) {
		std::set<uint_t *>::iterator itor = this-> unused_ids.end();
		--itor;

		__chunk_id = *itor;
		this-> unused_ids.erase(itor);
	} else {
		this-> chunk_info[this-> chunk_count].id, __chunk_id = (uint_t *)memory::mem_alloc(sizeof(uint_t));
		if (__chunk_id == NULL) {
			fprintf(stderr, "chunk_keeper: failed to alloc memory for 'chunk_id', errno: %d\n", errno);
			goto mem_clean;
		} else
			*this-> chunk_info[this-> chunk_count].id, *__chunk_id = this-> chunk_count;
	}

	chunk_info = &this-> chunk_info[*__chunk_id];
	chunk_data = &this-> chunk_data[*__chunk_id];

	chunk_data-> particles = static_cast<types::uni_par_t *>(memory::mem_alloc(this-> xaxis_len * this-> yaxis_len * this-> zaxis_len));
	if (chunk_data-> particles == NULL) {
		fprintf(stderr, "chunk_keeper: failed to alloc memory for chunk particles, errno: %d\n", errno);
		goto mem_clean;
	}

	rxaxis_len = this-> xaxis_len * UNI_PAR_XLEN;
	ryaxis_len = this-> yaxis_len * UNI_PAR_YLEN;
	rzaixs_len = this-> zaxis_len * UNI_PAR_ZLEN;
	if ((chunk_data-> pixmap = memory::alloc_pixmap(rxaxis_len, ryaxis_len, rzaixs_len)) == NULL) {
		fprintf(stderr, "chunk_keeper: failed to alloc pixmap for chunk, errno: %d\n", errno);
		goto mem_clean;
	}

	if (chunk_count == 1)
		memset(chunk_data-> pixmap, 244, (rxaxis_len * ryaxis_len * rzaixs_len) * 4);
	//if (chunk_count == 1)
	//	memset(chunk_data-> pixmap, 244, (rxaxis_len * ryaxis_len * rzaixs_len) * 4);

	this-> chunk_count ++;

	return FFLY_SUCCESS;
	mem_clean:

	if (this-> chunk_count == 0) {
		memory::mem_free(this-> chunk_info);
		memory::mem_free(this-> chunk_data);
	} else {
		this-> chunk_info = (types::chunk_info_t *)memory::mem_realloc(this-> chunk_info, (this-> chunk_count - 1) * sizeof(types::chunk_info_t));
		this-> chunk_data = (types::chunk_data_t *)memory::mem_realloc(this-> chunk_data, (this-> chunk_count - 1) * sizeof(types::chunk_data_t));
	}

	if (__chunk_id != NULL)
		memory::mem_free(__chunk_id);

	if (chunk_data-> particles != NULL)
		memory::mem_free(chunk_data-> particles);

	if (chunk_data-> pixmap != NULL)
		memory::mem_free(chunk_data-> pixmap);

	return FFLY_FAILURE;
}

boost::int8_t mdl::firefly::chunk_keeper::del_chunk(uint_t *__chunk_id, bool __hard) {
	if (__chunk_id == nullptr) return FFLY_NOP;

	types::chunk_info_t& chunk_info = this-> chunk_info[*__chunk_id];
	types::chunk_data_t& chunk_data = this-> chunk_data[*__chunk_id];

	if (__hard)
		memory::mem_free(chunk_info.id);

	memory::mem_free(chunk_data.particles);
	memory::mem_free(chunk_data.pixmap);

	if (__hard) {
		uint_t end_id = this-> chunk_count;
		this-> chunk_count --;

		if (*__chunk_id == end_id) return FFLY_SUCCESS;

		chunk_info = this-> chunk_info[end_id];
		chunk_data = this-> chunk_data[end_id];

		this-> chunk_info = (types::chunk_info_t *)realloc(this-> chunk_info, this-> chunk_count * sizeof(types::chunk_info_t));
		if (this-> chunk_info == NULL) {
			fprintf(stderr, "failed to relocate memory for 'chunk_info', errno: %d\n", errno);
			return FFLY_FAILURE;
		}

		this-> chunk_data = (types::chunk_data_t *)realloc(this-> chunk_data, this-> chunk_count * sizeof(types::chunk_data_t));
		if (this-> chunk_data == NULL) {
			fprintf(stderr, "failed to relocate memory for 'chunk_data', errno: %d\n", errno);
			return FFLY_FAILURE;
		}
	} else
		this-> unused_ids.insert(__chunk_id);

	return FFLY_SUCCESS;
}
