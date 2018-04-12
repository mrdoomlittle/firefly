# ifndef __ffly__file__h
# define __ffly__file__h
# include <mdlint.h>
# include "../types/size_t.h"
# include "../types/err_t.h"
# include "../types/off_t.h"
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

# define ffly_fileno(__file) \
    (__file)->fd

struct ffly_file {
	char const *path;
	mdl_int_t fd;
# ifndef __fflib
	FILE *libc_fp;
# endif
//	mdl_u8_t wbuf[], rbuf[];
};

# define FF_O_TRUNC O_TRUNC
# define FF_O_RDONLY O_RDONLY
# define FF_O_WRONLY O_WRONLY
# define FF_O_RDWR O_RDWR
# define FF_O_CREAT O_CREAT
# define FF_S_IRWXU S_IRWXU // r/w/x
# define FF_S_IRUSR S_IRUSR // r
# define FF_S_IWUSR S_IWUSR // w
# define FF_S_IXUSR S_IXUSR // e
# define FF_SEEK_SET SEEK_SET
# define FF_SEEK_CUR SEEK_CUR
# define FF_FILE struct ffly_file
# ifdef __cplusplus
extern "C" {
# endif
struct ffly_file* ffly_fopen(char const*, int, mdl_u32_t, ffly_err_t*);
ffly_err_t ffly_fcreat(char const*, mdl_u32_t);
ffly_err_t ffly_fstat(char const*, struct ffly_stat*);
ffly_err_t ffly_fwrite(struct ffly_file*, void*, mdl_uint_t);
ffly_off_t ffly_fseek(struct ffly_file*, ffly_off_t, int);
ffly_err_t ffly_fread(struct ffly_file*, void*, mdl_uint_t);
ffly_err_t ffly_fclose(struct ffly_file*);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__file__h*/
