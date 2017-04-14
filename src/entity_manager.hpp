# ifndef __entity__manager__hpp
# define __entity__manager__hpp
# include "obj_manager.hpp"
# include "system/vector.hpp"
# include "ui/camera.hpp"
# include "types/id_t.hpp"
# include "types/err_t.h"
# include "system/errno.h"
# include "types/coords_t.hpp"
# include "types/shape_info_t.hpp"
namespace mdl {
namespace firefly {
class entity_manager {
	types::err_t init(obj_manager *__obj_manager) {
		this-> _obj_manager = __obj_manager;
		return FFLY_SUCCESS;
	}

	types::__id_t add_entity(types::_3d_coords_t<> __coords, types::shape_info_t __shape_info, uint_t __xlen, uint_t __ylen, uint_t __zlen, types::err_t& __any_err) {
		types::__id_t obj_id = this-> _obj_manager-> add_obj(types::coords<>(__coords.xaxis, __coords.yaxis, __coords.zaxis), __shape_info, __xlen, __ylen, __zlen, __any_err);
		if (__any_err != FFLY_SUCCESS) return FFLY_FAILURE;

		this-> entity_indx.push_back() = obj_id;
	}

	private:
	system::vector<types::__id_t> entity_indx;
	obj_manager *_obj_manager = nullptr;
};
}
}

# endif /*__entity__manager__hpp*/
