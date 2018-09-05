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

struct segment {
	ff_u16_t dst;
	ff_u16_t src;
	ff_u16_t size;
};

struct header {
	ff_u32_t glyph_table;
	ff_uint_t glyph_c;
	ff_uint_t width, height;
	ff_uint_t seg;
};

# define E 2
# define N 6
struct typo_point points[] = {
	{0, 32},
	{8, 16},
	{16, 0},
	{32, 32},
	{24, 16},
	{16, 0},
	{24, 16},
	{8, 16}
};

int main() {
	printf("hello.\n");
	int fd;

	fd = open ("test.mcd", O_RDWR|O_TRUNC|O_CREAT, S_IRUSR|S_IWUSR);

	struct header hdr;
	hdr.glyph_table = sizeof(struct header);
	hdr.glyph_c = 1;
	hdr.width = 32;
	hdr.height = 32;

	struct segment seg;
	struct glyph g;
	g.idx = 0;
	g.code = sizeof(struct header)+sizeof(struct glyph)+((N+E)*sizeof(struct typo_point))+sizeof(struct segment);
	g.len = (N*(sizeof(ff_u16_t)+2))+2+2+1+(2*sizeof(ff_u16_t));
	printf("film size: %u, addr: %u\n", g.len, g.code);
	hdr.seg = sizeof(struct header)+sizeof(struct glyph);
	write(fd, &hdr, sizeof(struct header));
	write(fd, &g, sizeof(struct glyph));

	seg.dst = 0;
	seg.src = hdr.seg+sizeof(struct segment);
	seg.size = (N+E)*sizeof(struct typo_point);
	write(fd, &seg, sizeof(struct segment));
	write(fd, points, (N+E)*sizeof(struct typo_point));
	ff_uint_t i;
	ff_u8_t op;
	op = 1;
	write(fd, &op, 1);
	op = 1;
	write(fd, &op, 1);

	i = 0;
	while(i != N) {
		op = 1;
		write(fd, &op, 1);
		op = 0;
		write(fd, &op, 1);

		ff_uint_t addr;
		addr = i*sizeof(struct typo_point);
		write(fd, &addr, sizeof(ff_u16_t));
		i++;
	}

	op = 0;
	write(fd, &op, 1);

	ff_u16_t p0, p1;

	p0 = N*sizeof(struct typo_point);
	p1 = p0+sizeof(struct typo_point);
	write(fd, &p0, sizeof(ff_u16_t));
	write(fd, &p1, sizeof(ff_u16_t));

	op = 1;
	write(fd, &op, 1);
	op = 2;
	write(fd, &op, 1);
	close (fd);
}
