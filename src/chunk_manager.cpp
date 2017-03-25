# include "chunk_manager.hpp"
boost::int8_t mdl::firefly::chunk_manager::add_chunk(uint_t*& __chunk_id, uint_t __xaxis, uint_t __yaxis, uint_t __zaxis) {
	if (this-> _chunk_keeper.create_chunk(__chunk_id) != FFLY_SUCCESS) {
		fprintf(stderr, "chunk_manager: failed to create chunk.\n");
		return FFLY_FAILURE;
	}

	types::chunk_info_t& chunk_info = this-> _chunk_keeper.get_chunk_info(__chunk_id);

	chunk_info.coords.xaxis = __xaxis;
	chunk_info.coords.yaxis = __yaxis;
	chunk_info.coords.zaxis = __zaxis;

	this-> chunk_index.insert(__chunk_id);
}

boost::int8_t mdl::firefly::chunk_manager::rm_chunk(uint_t __xaxis, uint_t __yaxis, uint_t __zaxis) {
	uint_t *chunk_id = this-> find_chunk_id(__xaxis, __yaxis, __zaxis);

	if (this-> _chunk_keeper.del_chunk(chunk_id) != FFLY_SUCCESS) {
		fprintf(stderr, "chunk_manager: failed to delete chunk.\n");
		return FFLY_FAILURE;
	}
}
