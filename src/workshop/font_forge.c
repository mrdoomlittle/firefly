# include "font_forge.h"
# include "../ffint.h"
# include "../malloc.h"
struct dent {
	ff_u8_t r, g, b, a;
};

#define DENT_SHIFT 2
#define DENT_SIZE (1<<DENT_SHIFT)
static struct dent *map;
static ff_uint_t nd;
void static clear(void) {
	struct dent *d, *e;
	d = map;
	e = d+nd;
	while(d != e) {
		d->r = 0;
		d->g = 0;
		d->b = 0;
		d->a = 255;
		d++;
	}
}

#define dentat(__x, __y) \
	(map+(__x)+((__y)*(ws_width>>DENT_SHIFT)))


void ws_fontforge_init(void) {
	*(workshop.cu++) =  ws_fontforge_de_init;
	map = (struct dent*)malloc((nd = ((ws_width>>DENT_SHIFT)*(ws_height>>DENT_SHIFT)))*sizeof(struct dent));
}


void ws_fontforge_tick(void) {
	struct dent *d;
	ff_uint_t x, y;
	y = 0;
	while(y != (HEIGHT>>DENT_SHIFT)) {
		x = 0;
		while(x != (WIDTH>>DENT_SHIFT)) {
			d = dentat(x, y);
			ff_uint_t dx, dy;
			dx = x*DENT_SIZE;
			dy = y*DENT_SIZE;

			x++;
		}
		y++;
	}
}

void ws_fontforge_de_init(void) {
	free(map);
}

