# include <stdio.h>
# include <string.h>
# include "../ffint.h"
struct point {
	ff_int_t x, y;
};

# define _abs(__val) \
	((__val)<0?-(__val):__val)

# define A 10
# define WIDTH 64
# define HEIGHT 64
# define CPX

# ifdef CPX
# define N 7
# else
# define N 9
# endif
static char buf[WIDTH*HEIGHT];

void fix(struct point *__points, ff_int_t __x, ff_int_t __y) {
	ff_uint_t i;

	i = 0;
	ff_int_t t0, t1, t2, t3;
	char *p;
	p = buf+(__x+(__y*WIDTH));
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

		if ((__x == p0->x || __y == p0->y) && *p != '@') {
		//	*p = '^';
		}
		if ((__x == p0->x && __y == p0->y) || (__x == p1->x && __y == p1->y)) {
			*p = '@';
		}
	}

//	if ((t1-t0 > 0 && t2-t3 < 0) || (t0-t1 > 0 && t3-t2 < 0) && *p != '@') {
//		*p = '#';
//	}

	if (*p != '@') {
		char c;
		
		c = *p;

	//	if (!(t1>0) && t0>0) {
	//		c = '#';
	//	}


//		c = '0'+(t1&t3);
//		c = '0'+(t0&t2);

//		c = '0'+(!((t0&t1)&1));
		//c = '0'+t1

		ff_i8_t i, m;
		m = 0;
		if ((i = (t0-t1)) >=0) {
		//	m++;
		}

		if ((i = (t2-t3)) >=0) {
		//	m++;
		}


		if (m >0) {
			c = '0'+m;
		}

		ff_u8_t d0, d1;
		d0 = (t0^t1)&1;
		d1 = (t2^t3)&1;

		if (t0-t1 < 0 && t2-t3 < 0) {		
			c = '#';
		}
		
		if (t0-t1 > 0 && t2-t3 > 0) {
			c = '#';
		}
/*
		if (t0>t1 && t2>t3) {
			c = '#';
		}
		if (t0>t1 && t2>t3) {
			c = '$';
		}
*/
		*p = c;
	}	
}

# include <unistd.h>
int main() {
	struct point points[] = {
/*
		{4, 4},
		{27, 4},
		{27, 27},
		{4, 4},
		{27, 12}
*/
/*
		{4, 4},
		{16, 8},
		{27, 4},
		{16, 16},
		{4, 27},
		{4, 16},
		{4, 4},
*/
/*
		{2, 16},
		{29, 2},
		{29, 29},
		{2, 29},
		{23, 11},
		{2, 16}
*/
/*
		{2, 12},
		{29, 2},
		{29, 29},
		{2, 29},
		{26, 8},
		{2, 12}
*/
# ifdef CPX
		{2, 2},
		{29, 16},
		{2, 29},
		{29, 10},
		{5, 6},
		{2, 20},
		{2, 2}
# else
/*
	{29, 29},
	{29, 2},
	{2, 12},
	{23, 7},
	{20, 10},
	{23, 13},
	{2, 14},
	{29, 29}
*/
/*
    {29, 29},
    {16, 2},
    {2, 29},
    {6, 29},
    {13, 16}, // mid inner
    {13, 18},

    {16, 8},
    {18, 16}, // mid inner
    {18, 18},

    {25, 29},
    {29, 29},
*/

    {6, 2},
	{25, 2},
	{29, 6},
	{29, 25},
	{25, 29},
	{6, 29},
	{2, 25},
	{2, 6},
	{6, 2},
	
	{10, 6},
    {21, 6},
    {25, 10},
    {25, 21},
    {21, 25},

    {10, 25},
    {6, 21},
    {6, 10},
    {10, 6}
/*
    {29, 29},
    {16, 2},
    {2, 29},

    {6, 29},

    {10, 24}, // mid inner
//  {12, 28},

    {16, 8},

    {20, 20}, // mid inner

    {25, 29},
    {29, 29}
*/
# endif
	};

	ff_int_t x, y;

	ff_uint_t i;
	i = 0;
	while(i != N-1) {
		struct point *p;
		p = points+(i++);
		if (p->x > 32/2) {
			p->x+=A;
		}
		if (p->y > 32/2) {
			p->y+=A;
		}
	}

_again:
	memset(buf, '.', WIDTH*HEIGHT);
/*
	if (points[4].y >= 27) {
		points[4].y = 12;
	} else
		points[4].y++;
*/
	y = 0;
	while(y != HEIGHT) {
		x = 0;
		while(x != WIDTH) {
			fix(points, x, y);
			char c;
			c = buf[x+(y*WIDTH)];
			if (c == '@') {
				printf("\e[0;41m@\e[0;0m");
			} else
				printf("\e[0;40m%c\e[0;0m", c);
			x++;
		}
		printf("\n");
		y++;
	}
	usleep(100000);
	printf("\e[1J");
	goto _again;
}
