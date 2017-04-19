# include "uni_manager.hpp"

boost::int8_t mdl::firefly::uni_manager::init(uint_t __xaxis_split, uint_t __yaxis_split, uint_t __zaxis_split) {
	uint_t chunk_xlen = this-> xaxis_len / __xaxis_split;
	if (chunk_xlen % 1) {
		fprintf(stderr, "can't use %d for the chunk xaxis split.\n", __xaxis_split);
		return FFLY_FAILURE;
	}

	uint_t chunk_ylen = this-> yaxis_len / __yaxis_split;
	if (chunk_ylen % 1) {
		fprintf(stderr, "can't use %d for the chunk yaxis split.\n", __yaxis_split);
		return FFLY_FAILURE;
	}

	uint_t chunk_zlen = this-> zaxis_len / __zaxis_split;
	if (chunk_zlen % 1) {
		fprintf(stderr, "can't use %d for the chunk zaxis split.\n", __zaxis_split);
		return FFLY_FAILURE;
	}

	this-> chunk_xcount = __xaxis_split;
	this-> chunk_ycount = __yaxis_split;
	this-> chunk_zcount = __zaxis_split;
	uint_t chunk_count = chunk_xlen * chunk_ylen * chunk_zlen;
	this-> chunk_id_index = (uint_t **)memory::mem_alloc(chunk_count * sizeof(uint_t *));
	if (this-> chunk_id_index == NULL) {
		fprintf(stderr, "uni_manager: failed to alloc memory for 'chunk_id_index', errno: %d\n", errno);
		return FFLY_FAILURE;
	}

	this-> uni_pixmap = (types::pixmap_t *)memory::mem_alloc(chunk_count * sizeof(types::pixmap_t *));
	if (this-> uni_pixmap == NULL) {
		fprintf(stderr, "uni_manager: failed to alloc memory for 'uni_pixmap', errno: %d\n", errno);
		memory::mem_free(this-> chunk_id_index);
		return FFLY_FAILURE;
	}

	this-> uni_particles = (types::uni_par_t **)memory::mem_alloc(chunk_count * sizeof(types::uni_par_t *));
	if (this-> uni_particles == NULL) {
		fprintf(stderr, "uni_manager: failed to alloc memory for 'uni_particles', errno: %d\n", errno);
		memory::mem_free(this-> uni_pixmap);
		memory::mem_free(this-> chunk_id_index);
		return FFLY_FAILURE;
	}

	printf("uni_manager: chunk size has been set as %dx%dx%d\n", chunk_xlen, chunk_ylen, chunk_zlen);
	static chunk_manager _chunk_manager(chunk_xlen, chunk_ylen, chunk_zlen);
	this-> _chunk_manager = &_chunk_manager;
	uint_t chunk_point = 0;
	for (uint_t zaxis{}; zaxis != this-> zaxis_len; zaxis += chunk_zlen) {
		for (uint_t yaxis{}; yaxis != this-> yaxis_len; yaxis += chunk_ylen) {
			for (uint_t xaxis{}; xaxis != this-> xaxis_len; xaxis += chunk_xlen) {
				uint_t *chunk_id = nullptr;
				_chunk_manager.add_chunk(chunk_id, xaxis, yaxis, zaxis);
				types::chunk_data_t& chunk_data = _chunk_manager.chunk_data(chunk_id);

				this-> uni_pixmap[chunk_point] = chunk_data.pixmap;
				this-> uni_particles[chunk_point] = chunk_data.particles;
				this-> chunk_id_index[chunk_point] = chunk_id;
				chunk_point++;
			}
		}
	}
	printf("uni_manager: there are %d chunk in this uni.\n", chunk_point);
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::uni_manager::draw_chunk(uint_t __xfs, uint_t __yfs, uint_t __zfs, types::id_t __chunk_id, types::pixmap_t __pixbuff, uint_t __xaxis_len, uint_t __yaxis_len) {
	types::pixmap_t chunk_pixmap;
	if ((chunk_pixmap = this-> _chunk_manager-> chunk_data(__chunk_id).pixmap) == nullptr) return FFLY_FAILURE;
	graphics::draw_pixmap(__xfs, __yfs, __pixbuff, __xaxis_len, __yaxis_len, chunk_pixmap, this-> _chunk_manager-> get_chunk_xlen(), this-> _chunk_manager-> get_chunk_ylen());
}
