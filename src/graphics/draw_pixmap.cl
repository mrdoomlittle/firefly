# include <eint_t.h>

__kernel void draw_pixmap(__global uint_t *__xoffset, __global uint_t *__yoffset, __global uchar *__pixbuff, __global uint_t *__pb_xlen, __global uchar *__pixmap) {

	uint_t pixbuff_point = ((get_global_id(0) + (*__xoffset)) + ((get_global_id(1) + (*__yoffset)) * (*__pb_xlen))) * 4;
	uint_t pixmap_point = (get_global_id(0) + (get_global_id(1) * get_global_size(0))) * 4;
	//uint_t pixbuff_point = 0;

	__pixbuff[pixbuff_point] = __pixmap[pixmap_point];
	__pixbuff[pixbuff_point + 1] = __pixmap[pixmap_point + 1];
	__pixbuff[pixbuff_point + 2] = __pixmap[pixmap_point + 2];
	__pixbuff[pixbuff_point + 3] = __pixmap[pixmap_point + 3];
}
