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

# define N 9
struct typo_point points[] = {
/*
	{0, 32},
	{8, 16},
	{16, 0},
	{32, 32},
	{24, 16},
	{16, 0},
	{24, 16},
	{8, 16}
*/
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
	g.code = sizeof(struct header)+sizeof(struct glyph)+(N*sizeof(struct typo_point))+sizeof(struct segment);
	g.len = (1+sizeof(ff_u16_t)+sizeof(ff_u16_t))+4;
	printf("film size: %u, addr: %u\n", g.len, g.code);
	hdr.seg = sizeof(struct header)+sizeof(struct glyph);
	write(fd, &hdr, sizeof(struct header));
	write(fd, &g, sizeof(struct glyph));

	seg.dst = 0;
	seg.src = hdr.seg+sizeof(struct segment);
	seg.size = N*sizeof(struct typo_point);
	write(fd, &seg, sizeof(struct segment));
	write(fd, points, N*sizeof(struct typo_point));
	ff_uint_t i;
	ff_u8_t op;
	op = 1;
	write(fd, &op, 1);
	op = 1;
	write(fd, &op, 1);

	op = 2;
	write(fd, &op, 1);

	ff_u16_t n, adr;
	n = N;
	adr = 0;
	write(fd, &n, 2);
	write(fd, &adr, 2);

	op = 1;
	write(fd, &op, 1);
	op = 2;
	write(fd, &op, 1);
	close (fd);
}
