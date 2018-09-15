# include "../../ffint.h"
struct point {
	ff_int_t x, y;
};

# define N N0
struct point *points[0x100];
ff_uint_t np[0x100];

struct point one_points[] = {
# define PAD 2
# define TAL 166
# define THIC TAL/6
	{PAD, PAD},
	{THIC-PAD, PAD},
	{THIC-PAD, TAL-PAD},
	{PAD, TAL-PAD},
	{PAD, PAD}
# undef PAD
# undef TAL
# undef THIC
};

# define PAD 2
# define COR 14
# define TAL 166
# define THIC TAL/2
# define INNER 27
struct point zero_points[] = {
	{THIC-PAD, TAL-PAD-COR}, // right
	{THIC-PAD, PAD+COR},

	{THIC-PAD-COR, PAD}, // top
	{PAD+COR, PAD},

	{PAD, PAD+COR}, // left
	{PAD, TAL-PAD-COR},

	{PAD+COR, TAL-PAD}, // bottom
	{THIC-PAD-COR, TAL-PAD},

	{THIC-PAD, TAL-PAD-COR},

// inner part
	{(THIC-PAD)-INNER, (TAL-PAD-COR)-INNER}, // right
	{(THIC-PAD)-INNER, (PAD+COR)+INNER},

	{(THIC-PAD-COR)-INNER, PAD+INNER}, // top
	{(PAD+COR)+INNER, PAD+INNER},

	{PAD+INNER, (PAD+COR)+INNER}, // left
	{PAD+INNER, (TAL-PAD-COR)-INNER},

	{(PAD+COR)+INNER, (TAL-PAD)-INNER}, // bottom
	{(THIC-PAD-COR)-INNER, (TAL-PAD)-INNER},

	{(THIC-PAD)-INNER, (TAL-PAD-COR)-INNER},
};
# undef PAD
# undef COR
# undef TAL
# undef THIC

# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>

void one(void) {
# define N1 5
	np['1'] = 5;
	points['1'] = one_points;
}

void zero(void) {
# define N0 N1+18
	np['0'] = 18;
	points['0'] = zero_points;
}

int main() {
	int out;
	out = open("test.h", O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
	memset(points, 0, 0x100*sizeof(struct point*));
	char buf[1024];

	zero();
	one();
	ff_uint_t len;

	len = sprintf(buf, "# define N %u\n", N);
	write(out, buf, len);

	write(out, "struct typo_point points[] = {\n", 31);
	ff_uint_t i;
	i = 0;
	while(i != 0x100) {
		if (points[i] != NULL) {
			struct point *p;
			p = points[i];
			ff_uint_t n;
			n = np[i];
			
			ff_uint_t si;
			si = 0;
			while(si != n) {
				len = sprintf(buf, "\t{%u, %u},\n", p[si].x, p[si].y);
				write(out, buf, len);
				si++;
			}

		}

		i++;
	}
	write(out, "};\n", 3);
	close(out);
}
