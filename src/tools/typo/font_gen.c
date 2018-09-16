# include "../../ffint.h"
struct point {
	ff_int_t x, y;
};

# define N N0
struct point *points[0x100];
ff_uint_t np[0x100];

# include "zero.h"
# include "one.h"
# include "two.h"
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>

void two(void) {
# define N2 31
	np['2'] = 31;
	points['2'] = two_points;
}

void one(void) {
# define N1 N2+5
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
	two();
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
