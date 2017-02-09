# include <stdint.h>
# define BACKGROUND_COLOUR 0x0
__kernel void reset_background(__global uint8_t * __pixels) {
	__pixels[get_global_id(0)] = BACKGROUND_COLOUR;
}
