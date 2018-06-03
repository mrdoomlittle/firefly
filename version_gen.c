# include <stdio.h>
# include <sys/fcntl.h>
# include <unistd.h>
# include <sys/stat.h>
# define __ffly_source
# include "src/ffint.h"

ff_uint_t vst(char *__buf, ff_u64_t __ver) {
	char *p = __buf;
	char c0;
	
	__ver<<=4;
	ff_u8_t buf;
	ff_u8_t i = 0;
_again:
	buf = (__ver>>58&0x3f);
	c0 = 'a'+(buf>>3);
	buf-=((buf>>3)*(1<<3));
	
	*(p++) = c0;

	*(p++) = '0'+buf;

_sk:
	__ver<<=6;
	if (i++ != 9)
		goto _again;

	*(p++) = ';';
	*(p++) = '\n';
	return p-__buf;
}

int main() {
	int fd;

	ff_u8_t buf[1024];
	fd = open("version", O_RDWR);

	struct stat st;
	fstat(fd, &st);

	read(fd, buf, 1024);
	*(buf+st.st_size-1) = '\0';

	ff_u64_t val = 0;

	ff_u8_t *p = buf;

	char c0, c1;
	while((c0 = *(p++)) != ';') {
		if (c0 < 'a' || c0 > 'h') {
			printf("error.\n");
			break;
		}

		c1 = *(p++);
		if (c1 < '0' || c1 > '7') {
			printf("error.\n");
			break;
		}

		val = (val<<6)|(((c0-'a')<<3)+(c1-'0'));
	}

	printf("%u\n", val);

	ff_uint_t l;	
	l = vst(buf, val+1);
	*(buf+l) = '\0';
	printf("out: %s\n", buf);
	ftruncate(fd, 0);
	pwrite(fd, buf, l, 0);
	close(fd);
}
