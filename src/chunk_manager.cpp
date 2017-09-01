# include "chunk_manager.hpp"
# include "system/io.h"
mdl::firefly::types::err_t mdl::firefly::chunk_manager::init(uint_t __xa_len, uint_t __ya_len, uint_t __za_len) {
}

mdl::firefly::types::err_t mdl::firefly::chunk_manager::de_init() {
	types::err_t any_err;
	if ((any_err = this->ck.de_init()) != FFLY_SUCCESS) {
		system::io::printf(stderr, "chunk_manager: failed to de_init chunk_keeper.\n");
		return any_err;
	}
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::chunk_manager::add_cnk(types::id_t& __id, uint_t __xa, uint_t __ya, uint_t __za) {
	types::err_t any_err;
	if ((any_err = this->ck.create(__id, 0)) != FFLY_SUCCESS) {
		system::io::printf(stderr, "chunk_manager: failed to create chunk.\n");
		return any_err;
	}

	chunk_info& cnk_info = *this->ck.get_info(__id);
	cnk_info.coords.xa = __xa;
	cnk_info.coords.ya = __ya;
	cnk_info.coords.za = __za;

/*
	if ((any_err = this-> _chunk_keeper.create_cnk(__cnk_id)) != FFLY_SUCCESS) {
		fprintf(stderr, "chunk_manager: failed to create chunk.\n");
		return any_err;
	}

	types::chunk_info_t& cnk_info = this-> _chunk_keeper.get_cnk_info(__cnk_id);

	cnk_info.coords.xaxis = __xaxis;
	cnk_info.coords.yaxis = __yaxis;
	cnk_info.coords.zaxis = __zaxis;
	this-> cnk_indx.insert(__cnk_id);
*/
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::chunk_manager::rm_cnk(uint_t __xaxis, uint_t __yaxis, uint_t __zaxis) {
	
/*
	types::id_t cnk_id = this-> find_cnk_id(__xaxis, __yaxis, __zaxis);
	types::err_t any_err;
	if ((any_err = this-> _chunk_keeper.del_cnk(cnk_id)) != FFLY_SUCCESS) {
		fprintf(stderr, "chunk_manager: failed to delete chunk.\n");
		return any_err;
	}
*/
	return FFLY_SUCCESS;
}
