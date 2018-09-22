# include "mutate.h"
# include "../system/io.h"
void ffly_rgba_to_rgb(void *__src, void *__dst, ff_uint_t __npix) {
	ff_uint_t i;


	ff_u8_t *d, *s;
	i = 0;
	while(i != __npix) {
		d = ((ff_u8_t*)__dst)+(i*3);
		s = ((ff_u8_t*)__src)+(i*4);
		float alpha;

		alpha = s[3];
		*d = (ff_u8_t)(alpha*(((float)(*s))/255.));
		d[1] = (ff_u8_t)(alpha*(((float)s[1])/255.));
		d[2] = (ff_u8_t)(alpha*(((float)s[2])/255.));
		i++;
	}
}
