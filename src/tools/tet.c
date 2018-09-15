# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include "../ffint.h"
struct point {
	ff_int_t x, y;
};

# define _abs(__val) \
	((__val)<0?-(__val):__val)

# define A 0
# define WIDTH 243
# define HEIGHT 243
//# define CPX

# ifdef CPX
# define N 7
# else
# define N 5
# endif

ff_i8_t fix(struct point *__points, ff_int_t __x, ff_int_t __y) {
	ff_uint_t i;

	i = 0;
	ff_int_t t0, t1, t2, t3;
	t0 = 0;
	t1 = 0;
	t2 = 0;
	t3 = 0;
	struct point *p0, *p1, *p2;
	while(i != N-1) {
		p0 = __points+(i++);
		p1 = __points+i;
//		p2 = __points+i+1;

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

//		if ((__y >= p0->y && __y <= p1->y) || (__y <= p0->y && __y >= p1->y)) {
		if ((b2 >= 0 && b3 <= 0) || (b2 <= 0 && b3 >= 0)) {
			t0+=s>0;
			t1+=s<0;
		}

//		if ((__x >= p0->x && __x <= p1->x) || (__x <= p0->x && __x >= p1->x)) {
		if ((b0 >= 0 && b1 <= 0) || (b0 <= 0 && b1 >= 0)) {
			t2+=s>0;
			t3+=s<=0;
		}

	}		
	
	if (t0-t1 < 0 && t2-t3 < 0) {		
		return 0;
	}
		
	if (t0-t1 > 0 && t2-t3 > 0) {
		return 0;
	}

	return -1;
}

# define scale 0
# define tdown(__x) \
	(((__x))<<scale)
# define tup(__x) \
	((__x>>scale))
int main() {
	struct point points[] = {
# ifdef CPX
		{2, 2},
		{29, 16},
		{2, 29},
		{29, 10},
		{5, 6},
		{2, 20},
		{2, 2}
# else
# define PAD 2
# define TAL 166
# define THIC TAL/6
	{PAD, PAD},
	{THIC-PAD, PAD},
	{THIC-PAD, TAL-PAD},
	{PAD, TAL-PAD},
	{PAD, PAD}
# endif
	};

	ff_int_t x, y;

	int out;
	out = open("test.ppm", O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
	char header[64];
	char buf[128];
	ff_uint_t size;
	size = sprintf(buf, "P6\n%u %u\n255\n", tdown(WIDTH), tdown(HEIGHT));
	write(out, buf, size);
	ff_u8_t row[tdown(WIDTH)*3];
	y = 0;
	while(y != tdown(HEIGHT)) {
		x = 0;
		while(x != tdown(WIDTH)) {
			ff_u8_t in;
			in = fix(points, tup(x), tup(y));
			ff_u8_t *dst;

			dst = row+(x*3);
			ff_u8_t *r, *g, *b;
			r = dst;
			g = dst+1;
			b = dst+2;
			if (!in) {
				*r = 255;
				*g = 0;
				*b = 0;
			} else {
				*r = 0;
				*g = 0;
				*b = 0;
			}
			x++;
		}
		write(out, row, tdown(WIDTH)*3);
		y++;
	}
	close(out);
}
