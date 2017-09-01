# include "cal_dist.hpp"
mdl::uint_t mdl::firefly::maths::cal_dist(types::coords_t<> __obj_0, types::coords_t<> __obj_1) {
	uint_t obj_x0 = 0, obj_y0 = 0;
	uint_t obj_x1 = 0, obj_y1 = 0;

	if (__obj_0.xaxis > __obj_1.xaxis) {
		obj_x0 = __obj_0.xaxis;
		obj_x1 = __obj_1.xaxis;
	}

	if (__obj_1.xaxis > __obj_0.xaxis) {
		obj_x0 = __obj_1.xaxis;
		obj_x1 = __obj_0.xaxis;
	}

	if (__obj_0.yaxis > __obj_1.yaxis) {
		obj_y0 = __obj_0.yaxis;
		obj_y1 = __obj_1.yaxis;
	}

	if (__obj_1.yaxis > __obj_0.yaxis) {
		obj_y0 = __obj_1.yaxis;
		obj_y1 = __obj_0.yaxis;
	}

	double dist = squar_rt(squar(obj_x0-obj_x1)+squar(obj_y0-obj_y1));
	return (uint_t)round(dist);
}
