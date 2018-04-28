# include "light.h"
# include "../stdio.h"
# include "../maths/abs.h"
# define energy 0.8 // per pixel
void ffly_light_emit(ff_byte_t *__frame, ff_uint_t __width, ff_uint_t __height, ff_uint_t __x, ff_uint_t __y, ff_uint_t __z, float __intensity) {
	ff_uint_t x, y;

	float brightness;
	ff_uint_t radiance = (ff_uint_t)(__intensity*energy);

	ff_uint_t xr = __x+radiance;
	ff_uint_t yb = __y+radiance;

	if (__y<radiance)
		y = 0;
	else
		y = __y-radiance;
	while(y != yb) {
		if (__x<radiance)
			x = 0;
		else
			x = __x-radiance;
		while(x != xr) {
			ff_uint_t xidis = radiance-ffly_abs((ff_int_t)__x-(ff_int_t)x);
			ff_uint_t yidis = radiance-ffly_abs((ff_int_t)__y-(ff_int_t)y);
			float idis = ((float)xidis)+(((float)yidis)*0.5);
	
			ff_u8_t *p = __frame+((x+(y*__width))*4);
			ff_u8_t *r = p;
			ff_u8_t *g = p+1;
			ff_u8_t *b = p+2;
			*r = (ff_u8_t)(idis*(((float)*r)/(float)radiance));
			*g = (ff_u8_t)(idis*(((float)*g)/(float)radiance));
			*b = (ff_u8_t)(idis*(((float)*b)/(float)radiance));
			x++;	
		}
		y++;
	}
}
