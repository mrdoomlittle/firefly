# include <mdlint.h>
__kernel void render_camera(__global uchar *__uni, __global uchar *__camera, __global uint_t *__offsets, __global uint_t *__uni_xlen) {
	uint_t  uni_coords = ((get_global_id(0) + __offsets[0]) + ((get_global_id(1) + __offsets[1]) * (*__uni_xlen))) * 4;

	uint_t cam_coords = (get_global_id(0) + (get_global_id(1) * get_global_size(0))) * 4;
	__camera[cam_coords] = __uni[uni_coords];
	__camera[cam_coords + 1] = __uni[uni_coords + 1];
	__camera[cam_coords + 2] = __uni[uni_coords + 2];
	__camera[cam_coords + 3] = __uni[uni_coords + 3];

	//__camera[cam_coords + 1] = 244;
	//__camera[cam_coords + 3] = 244;
}
