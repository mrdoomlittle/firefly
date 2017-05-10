# include "chunk_manager.hpp"
mdl::firefly::types::err_t mdl::firefly::chunk_manager::add_cnk(types::id_t& __cnk_id, uint_t __xaxis, uint_t __yaxis, uint_t __zaxis) {
	types::err_t any_err;
	if ((any_err = this-> _chunk_keeper.create_cnk(__cnk_id)) != FFLY_SUCCESS) {
		fprintf(stderr, "chunk_manager: failed to create chunk.\n");
		return any_err;
	}

	types::chunk_info_t& cnk_info = this-> _chunk_keeper.get_cnk_info(__cnk_id);

	cnk_info.coords.xaxis = __xaxis;
	cnk_info.coords.yaxis = __yaxis;
	cnk_info.coords.zaxis = __zaxis;
	this-> cnk_indx.insert(__cnk_id);
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::chunk_manager::rm_cnk(uint_t __xaxis, uint_t __yaxis, uint_t __zaxis) {
	types::id_t cnk_id = this-> find_cnk_id(__xaxis, __yaxis, __zaxis);
	types::err_t any_err;
	if ((any_err = this-> _chunk_keeper.del_cnk(cnk_id)) != FFLY_SUCCESS) {
		fprintf(stderr, "chunk_manager: failed to delete chunk.\n");
		return any_err;
	}

	return FFLY_SUCCESS;
}
