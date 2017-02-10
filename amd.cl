# include <stdint.h>
# define BACKGROUND_COLOUR 0x0
__kernel void reset_background(__global uint8_t * __pixels, __global uint8_t * __background) {
	__pixels[get_global_id(0)] = __background[get_global_id(0)];
}
