# include "entity_manager.hpp"
mdl::firefly::types::err_t mdl::firefly::entity_manager::draw_cam(types::__id_t __entity_id, uint_t __xfs, uint_t __yfs, types::pixmap_t __pixbuff, uint_t __pb_xlen, uint_t __pb_ylen) {
	types::err_t any_err;
	if ((any_err = this->entity_indx[__entity_id].second->draw_camera(__xfs, __yfs, __pixbuff, __pb_xlen, __pb_ylen)) != FFLY_SUCCESS) return any_err;
	return FFLY_SUCCESS;
}

mdl::firefly::types::err_t mdl::firefly::entity_manager::manage(types::__id_t __entity_id) {
	if (this->entity_indx[__entity_id].second->inited) {
		types::__id_t obj_id = this->entity_indx[__entity_id].first;
		uint_t cnk_xlen = this->_uni_manager->get_cnk_xlen(), cnk_ylen = this->_uni_manager->get_cnk_ylen();

		uint_t enty_cxfs = floor(this->_obj_manager->get_xaxis(obj_id)/cnk_xlen)*cnk_xlen;
		uint_t enty_cyfs = floor(this->_obj_manager->get_yaxis(obj_id)/cnk_ylen)*cnk_ylen;

		uint_t enty_cxaxis = this->_obj_manager->get_xaxis(obj_id)-enty_cxfs;
		uint_t enty_cyaxis = this->_obj_manager->get_yaxis(obj_id)-enty_cyfs;

		int_t nw_cam_xaxis = ((int_t)enty_cxaxis-floor(this->entity_indx[__entity_id].second->get_xaxis_len()/2))+floor(this->_obj_manager->get_xaxis_len(obj_id)/2);
		if (nw_cam_xaxis > 0) this->entity_indx[__entity_id].second->set_xaxis(nw_cam_xaxis+enty_cxfs);
		else
			this->entity_indx[__entity_id].second->set_xaxis(enty_cxfs);

		int_t nw_cam_yaxis = ((int_t)enty_cyaxis-floor(this->entity_indx[__entity_id].second->get_yaxis_len()/2))+floor(this->_obj_manager->get_yaxis_len(obj_id)/2);
		if (nw_cam_yaxis > 0) this->entity_indx[__entity_id].second->set_yaxis(nw_cam_yaxis+enty_cyfs);
		else
			this->entity_indx[__entity_id].second->set_yaxis(enty_cyfs);


		printf("player yaxis: %d\n", this->_obj_manager->get_yaxis(obj_id));
/*
		uint_t cam_xaxis = this->entity_indx[__entity_id].second->get_xaxis();
		uint_t cam_yaxis = this->entity_indx[__entity_id].second->get_yaxis();

		uint_t cam_cxfs = floor(cam_xaxis/cnk_xlen)*cnk_xlen;
		uint_t cam_cyfs = floor(cam_yaxis/cnk_ylen)*cnk_ylen;

		if (cam_xaxis+this->entity_indx[__entity_id].second->get_xaxis_len() > cam_cxfs+cnk_xlen)
			this->entity_indx[__entity_id].second->set_xaxis(cam_cxfs+cnk_xlen-1);
		if (cam_yaxis+this->entity_indx[__entity_id].second->get_yaxis_len() > cam_cyfs+cnk_ylen)
			this->entity_indx[__entity_id].second->set_yaxis(cam_cyfs+cnk_ylen-1);
*/
		types::err_t any_err;
		if ((any_err = this->entity_indx[__entity_id].second->handle()) != FFLY_SUCCESS) return any_err;
	}
	return FFLY_SUCCESS;
}
