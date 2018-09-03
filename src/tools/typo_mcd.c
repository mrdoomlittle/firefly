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

# define N 4
struct typo_point points[] = {
	{0, 0},
	{32, 32},
	{24, 6},
	{0, 0}
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

	struct glyph g;
	g.idx = 0;
	g.code = sizeof(struct header)+sizeof(struct glyph);
	g.len = (N*(sizeof(struct typo_point)+2))+2+2;
	printf("film size: %u\n", g.len);
	write(fd, &hdr, sizeof(struct header));
	write(fd, &g, sizeof(struct glyph));
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

		write(fd, points+i, sizeof(struct typo_point));
		i++;
	}

	op = 1;
	write(fd, &op, 1);
	op = 2;
	write(fd, &op, 1);
	close (fd);
}
