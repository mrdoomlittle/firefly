# include "raster.h"
# include "../maths/max.h"
# include "../maths/min.h"
# include "../maths/abs.h"
# include "../system/io.h"
# include "raise.h"
ff_u8_t *typo_raster_bm;
ff_uint_t typo_raster_width;
ff_uint_t typo_raster_height;
ff_u8_t typo_raster_scale;
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

	p0->x<<=typo_raster_scale;
	p0->y<<=typo_raster_scale;
	p1->x<<=typo_raster_scale;
	p1->y<<=typo_raster_scale;


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
				*(typo_raster_bm+xx+(yy*typo_raster_width)) = 0xff;
			}

			x+=d;
			i++;
		}

	}
}

void ffly_typo_raster(void(**__op_tbl)(void)) {
	__op_tbl[0] = line;
}
