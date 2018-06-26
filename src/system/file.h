# ifndef __ffly__file__h
# define __ffly__file__h
# include "../ffint.h"
# include "../types.h"
# include "stat.h"

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
# include "mutex.h"
/*
	TODO:
		work more on this, write buffer, etc.
		allow for flags to be passed to write to bypass buffer and write
		directly to file, if error happens be wont be able to print
		as its going to get stuck in a loop
*/
# define ffly_fileno(__file) \
    (__file)->fd
# define OBUFSZ 76
struct ffly_file {
	char const *path;
	ff_u32_t fd;
# ifndef __fflib
	FILE *libc_fp;
# endif
	ff_u8_t flags;
	ff_u32_t bufdst;
	ff_i8_t drain;
	ff_u32_t off;
	ff_u8_t obuf[OBUFSZ];
	ff_u8_t *ob_p;
	ff_mlock_t lock;
};

#define FF_STREAM 0x1
#define FF_NOBUFF 0x2
#define FF_O_TRUNC O_TRUNC
#define FF_O_RDONLY O_RDONLY
#define FF_O_WRONLY O_WRONLY
#define FF_O_RDWR O_RDWR
#define FF_O_CREAT O_CREAT
#define FF_S_IRWXU S_IRWXU // r/w/x
#define FF_S_IRUSR S_IRUSR // r
#define FF_S_IWUSR S_IWUSR // w
#define FF_S_IXUSR S_IXUSR // e
#define FF_SEEK_SET SEEK_SET
#define FF_SEEK_CUR SEEK_CUR
#define FF_FILE struct ffly_file
# ifdef __cplusplus
extern "C" {
# endif
struct ffly_file* ffly_fopen(char const*, int, ff_u32_t, ff_err_t*);
void ffly_fopt(struct ffly_file*, ff_u8_t);
ff_err_t ffly_fcreat(char const*, ff_u32_t);
ff_err_t ffly_fstat(char const*, struct ffly_stat*);
ff_err_t ffly_fwrite(struct ffly_file*, void*, ff_uint_t);
ff_off_t ffly_fseek(struct ffly_file*, ff_off_t, int);
ff_err_t ffly_fread(struct ffly_file*, void*, ff_uint_t);
ff_err_t ffly_fclose(struct ffly_file*);
ff_err_t ffly_fdrain(struct ffly_file*);
// solid write
ff_err_t ffly_fwrites(struct ffly_file*, void*, ff_uint_t);
ff_err_t ffly_fpwrite(struct ffly_file*, void*, ff_uint_t, ff_uint_t);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__file__h*/
