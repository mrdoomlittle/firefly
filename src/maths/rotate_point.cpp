# include "rotate_point.hpp"

void mdl::firefly::maths::rotate_point(uint_t& __xaxis, uint_t& __yaxis, uint_t __angle, uint_t __xoffset, uint_t __yoffset) {
	float angle = (PI / 180.0) * (float)__angle;
	float sn = sinf(angle), cs = cosf(angle);

	float n_xaxis = (float)__xaxis * cs - (float)__yaxis * sn;
	float n_yaxis = (float)__yaxis * cs + (float)__xaxis * sn;

	if (n_xaxis < 0.0) {
		n_xaxis -= n_xaxis * 2;
		n_xaxis = (float)__xoffset - n_xaxis;
	} else n_xaxis = (float)__xoffset + n_xaxis;

	if (n_yaxis < 0.0) {
		n_yaxis -= n_yaxis * 2;
		n_yaxis = (float)__yoffset - n_yaxis;
	} else n_yaxis = (float)__yoffset + n_yaxis;

	__xaxis = (uint_t)roundf(n_xaxis);
	__yaxis = (uint_t)roundf(n_yaxis);
}
