# include <stdint.h>
__kernel void render_camera(__global uint8_t *__uni, __global uint8_t *__camera, __global ulong *__offsets, __global ulong *__uni_xlen) {
	int unsigned uni_coords = ((get_global_id(0) + __offsets[0]) + ((get_global_id(1) + __offsets[1]) * (*__uni_xlen))) * 4;

	int unsigned cam_coords = (get_global_id(0) + (get_global_id(1) * get_global_size(0))) * 4;
	__camera[cam_coords] = __uni[uni_coords];
	__camera[cam_coords + 1] = __uni[uni_coords + 1];
	__camera[cam_coords + 2] = __uni[uni_coords + 2];
	__camera[cam_coords + 3] = __uni[uni_coords + 3];

	//__camera[cam_coords + 1] = 244;
	//__camera[cam_coords + 3] = 244;
}
