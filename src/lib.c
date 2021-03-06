# include "lib.h"
# include "rand.h"
# include "linux/stat.h"
# include "linux/unistd.h"
# include "linux/fcntl.h"
# include "system/io.h"

ff_s32_t ff_mktemp(char *__tmpl) {
	char *p = __tmpl;
	while(*p++ != '.');
	
	ff_u32_t no = ffgen_rand32l();
	while(*p != '\0') {
		*(p++) = 'a'+(no&0xf);	
		no>>=4;
	}

	return open(__tmpl, O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
}
