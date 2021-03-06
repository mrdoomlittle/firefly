# include "raster.h"
# include "../maths/max.h"
# include "../maths/min.h"
# include "../maths/abs.h"
# include "../system/io.h"
# include "raise.h"
ff_u8_t *typo_raster_bm;
ff_uint_t typo_raster_width;
ff_uint_t typo_raster_height;
// ffly_typo_translate not work on line
ff_u8_t extern *raise_p;
void static
line_dsc(struct typo_point *__p0, struct typo_point *__p1) {
	if (__p0->x == __p1->x) {
		ff_int_t y, x;
		y = __p0->y;
		x = __p0->x;
		while(y <= __p1->y) {
			*(typo_raster_bm+x+(y*typo_raster_width)) = 0xff;
			y++;
		}
		return;
	}

	ff_int_t dx, dy;

	dx = __p1->x-__p0->x;
	dy = __p1->y-__p0->y;

	ff_int_t d;
	if (dx<0)
		d = -1;
	else
		d = 1;

	ff_int_t xmax, ymax;
	xmax = ffly_abs(dx);
	ymax = ffly_abs(dy);

	ff_int_t x, y;
	x = 0;
	y = 0;

	double deltaerr;

	deltaerr = ffly_abs((double)dy/(double)dx);
	double error;
	error = 0.0;
	ff_uint_t i;
	i = 0;
	while(i <= xmax && y <= ymax) {
		error+=deltaerr;
		ff_int_t xx, yy;
    _again:
		if (y>ymax)
			break;
		xx = (__p0->x+x);
		yy = (__p0->y+y);

		if (xx < typo_raster_width && yy < typo_raster_height) {
			*(typo_raster_bm+xx+(yy*typo_raster_width)) = 0xff;
		}

		if (error>0) {
			y++;
			error = error-1.0;
            goto _again;
		}
        
		x+=d;
		i++;
	}
}

void static
line_asc(struct typo_point *__p0, struct typo_point *__p1) {
	line_dsc(__p1, __p0);
}

void static line(void) {
	struct typo_point *p0, *p1;
	p0 = (struct typo_point*)(raise_stack+*(ff_u16_t*)raise_p);
	p1 = (struct typo_point*)(raise_stack+*(ff_u16_t*)(raise_p+sizeof(ff_u16_t)));

	if (p0->y<p1->y) {
		line_dsc(p0, p1);
	} else if (p0->y>p1->y) {
		line_asc(p0, p1);
	} else {
		ff_int_t x, y;

		y = p0->y;
		x = 0;
		ff_int_t dx, xmax;

		dx = p1->x-p0->x;

		xmax = ffly_abs(dx);
		ff_int_t d;

		if (dx<0)
			d = -1;
		else
			d = 1;

		ff_uint_t i;
		i = 0;
		while(i <= xmax) {
			ff_int_t xx, yy;
  
  			xx = (p0->x+x);
			yy = y;
			if (xx < typo_raster_width && yy < typo_raster_height) {
				*(typo_raster_bm+xx+(yy*typo_raster_width)) = 255;
			}

			x+=d;
			i++;
		}

	}
}

/*
	okay this is shit but works minor issues where x and y of diffrent points intersect and light the point up but nothing that cant be fixed later.


	*				
	.			
	.		
	.			
	.			
	.
	X...................*

	* = one point
	X = issue i think i know why but cant be bothered to act as it only minor
*/
ff_i8_t static
poly(struct typo_point *__b, ff_uint_t __n, ff_int_t __x, ff_int_t __y) {
	ff_uint_t i;
	ff_int_t t0, t1, t2, t3;
	t0 = 0;
	t1 = 0;
	t2 = 0;
	t3 = 0;
	i = 0;

	struct typo_point *p0, *p1;
	while(i != __n-1) {
		p0 = __b+(i++);
		p1 = __b+i;

		ff_int_t x0, y0;
		ff_int_t x, y;

		x0 = p1->x-p0->x;
		y0 = p1->y-p0->y;

		x = __x-p0->x;
		y = __y-p0->y;

		ff_int_t k0, k1;
		k0 = x*y0;
		k1 = x0*y;

		ff_int_t s;
		s = k0-k1;

		ff_int_t b0, b1, b2, b3;
		b0 = __x-p0->x;
		b1 = __x-p1->x;
		b2 = __y-p0->y;
		b3 = __y-p1->y;

		if ((b2 >=0 && b3<=0) || (b2 <=0 && b3>=0)) {
			t0+=s>0;
			t1+=s<0;
		}

		if ((b0 >=0 && b1<=0) || (b0 <=0 && b1>=0)) {
			t2+=s>0;
			t3+=s<0;
		}

		// debug
		if ((__x == p0->x && __y == p0->y) || (__x == p1->x && __y == p1->y)) {
			return -1;
		}
	}

	if (t0-t1 > 0 && t2-t3 > 0) {
		return 0;
	}
	if (t0-t1 < 0 && t2-t3 < 0) {
		return 0;
	}

	return -1;
}

void static
poly_draw(void) {
	struct typo_point *b;
	ff_u16_t n;
	n = *(ff_u16_t*)raise_p;
	b = (struct typo_point*)(raise_stack+*(ff_u16_t*)(raise_p+sizeof(ff_u16_t)));
	ffly_printf("drawing poly: %u vert.\n", n);

	ff_uint_t x, y;

	y = 0;
	while(y != typo_raster_height) {
		x = 0;
		while(x != typo_raster_width) {
			if (!poly(b, n, ffly_typo_translate_from(x), ffly_typo_translate_from(y))) {
				char *p;
				 p = typo_raster_bm+x+(y*typo_raster_width);
				 *p = ~*p;
			}
			x++;
		}
		y++;
	}
}

void ffly_typo_raster(void(**__op_tbl)(void)) {
	__op_tbl[0] = line;
	__op_tbl[2] = poly_draw;
}
