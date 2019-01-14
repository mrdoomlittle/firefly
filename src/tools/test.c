# include <stdio.h>
# include "../ffint.h"

# define dot(__x0, __x1, __y0, __y1) \
		((__x0*__x1)+(__y0*__y1))

struct nt_vertex2 {
	ff_s32_t x, y;
};

static ff_s32_t x0, y0, z0;
static ff_s32_t x1, y1, z1;
static ff_s32_t x2, y2, z2;
ff_i8_t
static dim(ff_int_t __x, ff_int_t __y) {
	struct nt_vertex2 v0 = {
		.x=x1-x0,
		.y=y1-y0
	};

	struct nt_vertex2 v1 = {
		.x=x2-x0,
		.y=y2-y0
	};

	struct nt_vertex2 v2 = {
		.x=__x-x0,
		.y=__y-y0
	};

	double x, y;
	x = v2.x;
	y = v2.y;

	double f0, f1, f2, f3, f4, f5;

	f0 = x*(y2-y0);
	f1 = (x2-x0)*y;

	f2 = x*(y0-y1);
	f3 = (x0-x1)*y;

	f4 = (v1.x-x)*(y2-y1);
	f5 = (x2-x1)*(v1.y-y);

	if ((f0-f1)*(f2-f3) >= 0 && (f2-f3)*(f4-f5) >= 0 && (f0-f1)*(f4-f5) >= 0)
		return 0;
	return -1;
}

#define WIDTH 12
#define HEIGHT 16
void draw(void) {
	ff_u32_t x, y;
	y = 0;
	while(y != HEIGHT) {
		x = 0;
		while(x != WIDTH) {
			if (!dim(x, y)) {
				double a0, a1, a2, a3, a4, a5;
				a0 = x-x0;
				a1 = y-y0;
				a2 = x1-a0;
				a3 = y1-a1;
				a4 = a0-x2;
				a5 = y2-a1;

				double b0, b1, b2, b3, b4, b5;
				double d0, d1;
				d0 = dot(x1, x1, y1, y1);
				d1 = dot(x2, x2, y2, y2);
				b4 = ((float)z0)/d0;
	
				b0 = (d0-dot(x, x1, y, y1))*b4;
				b0 = b0/d1;

				b0 = (d1-dot(x, x2, y, y2))*b0;

//				b5 = b0/d1;
//				b1 = (d1-dot(x, x2, y, y2))*b5;
//				b4 = ((float)z0)/(((x1-x0)*(x1-x0))+((y1-y0)*(y1-y0)));
//				b0 = ((a0*(x1-x0))+(a1*(y1-y0)))*b4;
	
		//		b4 = ((float)z0)/(x1-x0);
		//		b0 = (((x1-x0)-a0)*b4)/(y2-y0);
		//		b0 = a3*b0;
	
				

				double w;
				w = b0;			
				printf("%lf . ", w);
			}
			x++;
		}
		printf("\n");
		y++;
	}
}


int main(void) {
#ifdef PROF0
	x0 = 0;
	y0 = 0;
	x1 = WIDTH;
	y1 = HEIGHT;
	x2 = 0;
	y2 = HEIGHT;
#else
	x0 = 0;
	y0 = 0;
	x1 = WIDTH;
	y1 = HEIGHT;
	x2 = HEIGHT;
	y2 = 0;
#endif
	z0 = 1;
	z1 = 0;
	z2 = 0;
	draw();
}
