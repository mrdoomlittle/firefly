# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include "../ffint.h"

struct typo_point {
	ff_int_t x, y;
};

struct glyph {
	ff_u16_t idx;
	ff_u32_t code;
	ff_uint_t len;
};

struct header {
	ff_u32_t glyph_table;
	ff_uint_t glyph_c;
	ff_uint_t width, height;
};

struct line {
	struct typo_point p0, p1;	
};

# define N 1
struct line lines[] = {
/*
	{{3, 0}, {1, 3}},
	{{3, 0}, {4, 3}},
	{{1, 3}, {0, 5}},
	{{4, 3}, {5, 5}},
	{{1, 3}, {4, 3}},
*/
//	{{5, 17}, {12, 17}},
//	{{17, 0}, {12, 17}},
//	{{17, 0}, {5, 17}}
};

int main() {
	printf("hello.\n");
	int fd;

	fd = open ("test.mcd", O_RDWR|O_TRUNC|O_CREAT, S_IRUSR|S_IWUSR);

	struct header hdr;
	hdr.glyph_table = sizeof(struct header);
	hdr.glyph_c = 1;
	hdr.width = 6;
	hdr.height = 6;

	struct glyph g;
	g.idx = 0;
	g.code = sizeof(struct header)+sizeof(struct glyph);
	g.len = N*((2*sizeof(struct typo_point))+1);
	write(fd, &hdr, sizeof(struct header));
	write(fd, &g, sizeof(struct glyph));
	ff_uint_t i;

	i = 0;
	while(i != N) {
	ff_u8_t op;
	op = 0;
	write(fd, &op, 1);

	struct typo_point p;
	p.x = lines[i].p0.x;
	p.y = lines[i].p0.y;
	write(fd, &p, sizeof(struct typo_point));

	p.x = lines[i].p1.x;
	p.y = lines[i].p1.y;
	write(fd, &p, sizeof(struct typo_point));
	i++;
	}
	close (fd);
}
