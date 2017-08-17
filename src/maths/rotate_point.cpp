# include "rotate_point.hpp"
void mdl::firefly::maths::rotate_point(int_t& __xaxis, int_t& __yaxis, u16_t __angle, i8_t __dir) {
	double rad = (double)__angle*(PI/180.0f);
	double sn = sin(rad), cs = cos(rad);

	double n_xaxis, n_yaxis;
	if (__dir == -1) {
		n_xaxis = (double)__xaxis*cs+(double)__yaxis*sn;
		n_yaxis = -(double)__xaxis*sn+(double)__yaxis*cs;
	} else if (__dir == 1) {
		n_xaxis = -(double)__xaxis*cs+(double)__yaxis*sn;
		n_yaxis = (double)__xaxis*sn+(double)__yaxis*cs;
 	}

	__xaxis = (int_t)round(n_xaxis);
	__yaxis = (int_t)round(n_yaxis);
}
