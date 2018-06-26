# include <stdio.h>
# include <sys/fcntl.h>
# include <unistd.h>
# include <sys/stat.h>
# define __ffly_source
# include "src/ffint.h"
# include "src/system/util/ff6.h"
# include <string.h>
# include "src/version.h"
# include "src/system/string.h"
ff_uint_t gen_c_vers(ff_u8_t *__buf, char *__vers, ff_uint_t __len) {
	ff_u8_t *bufp = __buf;
	char *p = __vers;
	char *end = p+__len;

	*(bufp++) = '{';
	while(p != end-1) {
		*(bufp++) = 0x27;
		*(bufp++) = *(p++);
		*(bufp++) = 0x27;
		*(bufp++) = ',';
		*(bufp++) = ' ';
	}
	
	*(bufp++) = 0x27;
	*(bufp++) = *(p++);
	*(bufp++) = 0x27;

	*(bufp++) = '}';
	return bufp-__buf;
}

void out_c_hdr(ff_u8_t *__buf, char *__vers, ff_uint_t __len, ff_u64_t __vers_int) {
	int fd;
	fd = open("src/version.h", O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
	char const p0[] = {'#', 'i', 'f', 'n', 'd', 'e', 'f', ' ', '_', '_', 'f', 'f', 'l', 'y', '_', '_', 'v', 'e', 'r', 's', 'i', 'o', 'n', '_', '_', 'h', '\n'};
	char const p1[] = {'#', 'd', 'e', 'f', 'i', 'n', 'e', ' ', '_', '_', 'f', 'f', 'l', 'y', '_', '_', 'v', 'e', 'r', 's', 'i', 'o', 'n', '_', '_', 'h', '\n'};
	char const p2[] = {'#', 'd', 'e', 'f', 'i', 'n', 'e', ' ', 'f', 'f', 'l', 'y', '_', 'v', 'e', 'r', 's', 'i', 'o', 'n'};
	char const p3[] = {'#', 'd', 'e', 'f', 'i', 'n', 'e', ' ', 'f', 'f', 'l', 'y', '_', 'v', 'e', 'r', 's', 'i', 'o', 'n', '_', 'l', 'e', 'n'};
	char const p4[] = {'#', 'd', 'e', 'f', 'i', 'n', 'e', ' ', 'f', 'f', 'l', 'y', '_', 'v', 'e', 'r', 's', 'i', 'o', 'n', '_', 'i', 'n', 't'};
	char const p5[] = {'#', 'd', 'e', 'f', 'i', 'n', 'e', ' ', 'f', 'f', 'l', 'y', '_', 'v', 'e', 'r', 's', 'i', 'o', 'n', '_', 's', 't', 'r'};
	char const p6[] = {'#', 'e', 'n', 'd', 'i', 'f', ' ', '/', '*', '_', '_', 'f', 'f', 'l', 'y', '_', '_', 'v', 'e', 'r', 's', 'i', 'o', 'n', '_', '_', 'h', '*', '/', '\n'};

	ff_u8_t *p = __buf;
	ff_uint_t l;
	l = sizeof(p0);
	memcpy(p, p0, l);
	p+=l;

	l = sizeof(p1);
	memcpy(p, p1, l);
	p+=l;

	l = sizeof(p2);
	memcpy(p, p2, l);
	p+=l;
	*(p++) = ' ';
	p+=gen_c_vers(p, __vers, __len);
	*(p++) = '\n';

	l = sizeof(p3);
	memcpy(p, p3, l);
	p+=l;
	*(p++) = ' ';
	*(p++) = '2';
	*(p++) = '2';
	*(p++) = '\n';

	l = sizeof(p4);
	memcpy(p, p4, l);
	p+=l;
	*(p++) = ' ';
	p+=ffly_nots(__vers_int, p);
	*(p++) = '\n';

	l = sizeof(p5);
	memcpy(p, p5, l);
	p+=l;
	*(p++) = ' ';
	*(p++) = '"';
	memcpy(p, __vers, __len);
	p+=__len;
	*(p++) = '"';
	*(p++) = '\n';

	l = sizeof(p6);
	memcpy(p, p6, l);
	p+=l;

	write(fd, __buf, p-__buf);
	close(fd);
}

int main() {
	int fd;
	ff_u8_t vers[1024];
	ff_u8_t buf[1024];
	fd = open("version", O_RDWR);

	struct stat st;
	fstat(fd, &st);

	read(fd, buf, 1024);
	*(buf+st.st_size-1) = '\0';

	ff_u64_t val = 0;
	ffly_ff6_dec(buf, &val, st.st_size-1);
	printf("%u\n", val);

	ff_uint_t l;
	val++;
	l = ffly_ff6_enc(&val, vers, sizeof(ff_u64_t));
	*(vers+l) = '\0';
	printf("out: %s\n", vers);
	ftruncate(fd, 0);
	pwrite(fd, vers, l, 0);
	close(fd);

	out_c_hdr(buf, vers, l, val);
}
