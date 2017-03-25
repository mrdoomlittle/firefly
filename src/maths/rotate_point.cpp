# include "rotate_point.hpp"
# include <cstdio>
void mdl::firefly::maths::rotate_point(uint_t& __xaxis, uint_t& __yaxis, boost::uint16_t __angle, uint_t __xoffset, uint_t __yoffset, boost::int8_t __dir) {
	float rad = (float)__angle * (PI / 180.0f);
	float sn = sinf(rad), cs = cosf(rad);

	float n_xaxis, n_yaxis;
	if (__dir == -1) {
		n_xaxis = (float)__xaxis * cs + (float)__yaxis * sn;
		n_yaxis = -(float)__xaxis * sn + (float)__yaxis * cs;
	} else if (__dir == 1) {
		n_xaxis = -(float)__xaxis * cs + (float)__yaxis * sn;
		n_yaxis = (float)__xaxis * sn + (float)__yaxis * cs;
 	}

	if (n_xaxis < 0.0f) {
		n_xaxis -= n_xaxis * 2;
		n_xaxis = (float)__xoffset - n_xaxis;
	} else n_xaxis = (float)__xoffset + n_xaxis;

	if (n_yaxis < 0.0f) {
		n_yaxis -= n_yaxis * 2;
		n_yaxis = (float)__yoffset - n_yaxis;
	} else n_yaxis = (float)__yoffset + n_yaxis;

	__xaxis = (uint_t)roundf(n_xaxis);
	__yaxis = (uint_t)roundf(n_yaxis);
}
