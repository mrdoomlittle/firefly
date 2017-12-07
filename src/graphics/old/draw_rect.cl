# include <mdlint.h>
# include <colour_t.h>

__kernel void draw_rect(__global uchar *__pixbuff, __global uint_t *__b_xcoord, __global uint_t *__b_ycoord, __global uint_t *__e_xcoord, __global uint_t *__e_ycoord, __global colour_t *__colour, __global uint_t *__pb_xlen) {
	uint_t pixbuff_point = ((get_global_id(0) + (*__b_xcoord)) + ((get_global_id(1) + (*__b_ycoord)) * (*__pb_xlen))) * 4;

	__pixbuff[pixbuff_point] = __colour-> r;
	__pixbuff[pixbuff_point + 1] = __colour-> g;
	__pixbuff[pixbuff_point + 2] = __colour-> b;
	__pixbuff[pixbuff_point + 3] = __colour-> a;
}
