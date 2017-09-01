# ifndef __entity__manager__hpp
# define __entity__manager__hpp
# include "obj_manager.hpp"
# include "system/vec.h"
# include "ui/camera.hpp"
# include "types/id_t.h"
# include "types/err_t.h"
# include "system/errno.h"
# include "types/coords_t.hpp"
# include "types/pixmap_t.h"
# include "types/shape_info_t.hpp"
# include <utility>
# include "data/pair.hpp"
namespace mdl {
namespace firefly {
class entity_manager {
	public:
	types::err_t init(obj_manager *__obj_manager, uni_manager *__uni_manager) {
		this->_obj_manager = __obj_manager;
		this->_uni_manager = __uni_manager;
		return FFLY_SUCCESS;
	}

	types::__id_t add_entity(types::_3d_coords_t<> __coords, types::shape_info_t __shape_info, uint_t __xlen, uint_t __ylen, uint_t __zlen, types::err_t& __any_err) {
		types::__id_t obj_id = this->_obj_manager->add_obj(types::coords<>(__coords.xaxis, __coords.yaxis, __coords.zaxis), __shape_info, __xlen, __ylen, __zlen, __any_err);
		if (__any_err != FFLY_SUCCESS) return FFLY_FAILURE;

		types::__id_t entity_id = this->entity_indx.size();
		this->entity_indx.resize(1, 1);

		this->entity_indx.begin()->first = obj_id;
		this->entity_indx.begin()->second = (ui::camera*)memory::mem_alloc(sizeof(ui::camera));
		*this->entity_indx.begin()->second = ui::camera(this->_uni_manager);
		return entity_id;
	}

	types::err_t rm_entity(types::__id_t __entity_id) {
		this->entity_indx.swp(__entity_id, this->entity_indx.end());
		this->entity_indx.resize(1, -1);
		return FFLY_SUCCESS;
	}

	types::err_t init_cam(types::__id_t __entity_id, uint_t __xaxis_len, uint_t __yaxis_len) {
		this->entity_indx[__entity_id].second->init(__xaxis_len, __yaxis_len);
	}

	types::err_t de_init_cam(types::__id_t __entity_id) {
		this->entity_indx[__entity_id].second->de_init();
	}

	types::err_t draw_cam(types::__id_t __entity_id, uint_t __xfs, uint_t __yfs, types::pixmap_t __pixbuff, uint_t __pb_xlen, uint_t __pb_ylen);
	types::err_t manage(types::__id_t __entity_id);
	types::err_t manage() {
		for (types::__id_t entity_id{}; entity_id != this->entity_indx.size(); entity_id ++) this->manage(entity_id);
	}

	void pull_xaxis(types::__id_t __entity_id, uint_t __amount) {this->_obj_manager->pull_xaxis(this->entity_indx[__entity_id].first, __amount);}
	void push_xaxis(types::__id_t __entity_id, uint_t __amount) {this->_obj_manager->push_xaxis(this->entity_indx[__entity_id].first, __amount);}
	void pull_yaxis(types::__id_t __entity_id, uint_t __amount) {this->_obj_manager->pull_yaxis(this->entity_indx[__entity_id].first, __amount);}
	void push_yaxis(types::__id_t __entity_id, uint_t __amount) {this->_obj_manager->push_yaxis(this->entity_indx[__entity_id].first, __amount);}
	private:
	system::vec<data::pair<types::__id_t, ui::camera *>> entity_indx;
	obj_manager *_obj_manager = nullptr;
	uni_manager *_uni_manager = nullptr;
};
}
}

# endif /*__entity__manager__hpp*/
