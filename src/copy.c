# include "copy.h"
# include "ffint.h"
# include "system/error.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "linux/unistd.h"
# include "linux/fcntl.h"
# include "linux/stat.h"
# include "system/io.h"
# define CHUNK_SHIFT 8//256
# define CHUNK_SIZE (1<<CHUNK_SHIFT)

/*
	NOTE:
		
*/
ff_err_t
ff_fcopy(char const *__src, char const *__dst) {
	ff_u8_t *buf = (ff_u8_t*)__ffly_mem_alloc(CHUNK_SIZE);
	struct stat st;
	int s, d;

	if ((s = open(__src, O_RDONLY, 0)) == -1) {
		ffly_fprintf(ffly_err, "failed to open file: %s\n", __dst);
		reterr;
	}

	if ((d = open(__dst, O_WRONLY|O_CREAT|O_TRUNC,
		S_IRUSR|S_IWUSR)) == -1) {
		ffly_fprintf(ffly_err, "failed to open file: %s\n", __dst);
		close(s);
		reterr;
	}
	
	fstat(s, &st);

	ff_uint_t cc = st.st_size>>CHUNK_SHIFT;
	ff_uint_t left = st.st_size-(cc*CHUNK_SIZE);
	if (left>0) {
		read(s, buf, left);
		write(d, buf, left);
	}
_again:
	if (cc>0) {
		read(s, buf, CHUNK_SIZE);
		write(d, buf, CHUNK_SIZE);
		goto _again;
	}

	close(s);
	close(d);
	__ffly_mem_free(buf);
}
