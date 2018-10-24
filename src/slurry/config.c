# include "slurry.h"
# ifdef __fflib
# include "../linux/stat.h"
# include "../linux/fcntl.h"
# include "../linux/unistd.h"
# else
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# endif
# include "../malloc.h"
# include "../stdio.h"
ff_uint_t static
po10_t[] = {
	1,
	10,
	100,
	1000,
	10000,
	100000
};

ff_uint_t static
stdec(char *__buf, ff_uint_t __len) {
	ff_uint_t r;

	r = 0;

	char *p, *end;
	p = __buf;
	end = p+__len;
	ff_uint_t i;
	i = __len;
	while(p != end) {
		r+=(*(p++)-'0')*po10_t[--i];
	}
	return r;
}

char static *g_buf;
ff_uint_t static g_len;

ff_uint_t static port_n;
void static
port_num(void) {
	port_n = stdec(g_buf, g_len);
	// set
	s_ov[OV_PORTN] = &port_n;
}

static void(*imp[])(void) = {
	port_num
};

#define is_space(__c)\
	(__c == ' ' || __c == '\t' || __c == '\n')
void s_confload(void) {
	int fd;
	fd = open("slurry.conf", O_RDONLY, 0);
	if (fd<0)
		return;

	struct stat st;
	fstat(fd, &st);
	
	char *p, *m;
	m = p = (char*)malloc(st.st_size);
	read(fd, p, st.st_size);

	char b0[8], b1[128];
	ff_uint_t i, id;
_again:
	//ignore white spaces
	while(is_space(*p)) p++;
	if (*(p++) == '>') {
		char c;
		i = 0;
		while((c = *(p++)) != ':') {
			b0[i++] = c;
		}

		id = stdec(b0, i);

		i = 0;
		while((c = *(p++)) != ',') {
			b1[i++] = c;
		}

		g_buf = b1;
		g_len = i;
		imp[id]();
		goto _again;
	}
	free(m);
	close(fd);
}
