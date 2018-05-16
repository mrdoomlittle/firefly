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

/*
	TODO:
		work more on this, write buffer, etc.
*/
# define ffly_fileno(__file) \
    (__file)->fd

struct ffly_file {
	char const *path;
	ff_u32_t fd;
# ifndef __fflib
	FILE *libc_fp;
# endif
};

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
ff_err_t ffly_fcreat(char const*, ff_u32_t);
ff_err_t ffly_fstat(char const*, struct ffly_stat*);
ff_err_t ffly_fwrite(struct ffly_file*, void*, ff_uint_t);
ff_off_t ffly_fseek(struct ffly_file*, ff_off_t, int);
ff_err_t ffly_fread(struct ffly_file*, void*, ff_uint_t);
ff_err_t ffly_fclose(struct ffly_file*);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__file__h*/
