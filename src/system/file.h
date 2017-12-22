# ifndef __ffly__file__h
# define __ffly__file__h
# include "../types/size_t.h"
# include <sys/types.h>
# include <fcntl.h>
# include "../types/err_t.h"
# include "../types/off_t.h"
struct ffly_file {
	char const *path;
	int d;
};


struct ffly_stat {
	ffly_size_t size;
};

# define FF_O_RDONLY O_RDONLY
# define FF_O_WRONLY O_WRONLY
# define FF_O_CREATE O_CREAT
# define FF_S_IRWXU S_IRWXU
# define FF_S_IRUSR S_IRUSR
# define FF_S_IWUSR S_IWUSR
# define FF_S_IXUSR S_IXUSR
# define FF_SEEK_SET SEEK_SET
# define FF_FILE struct ffly_file
# ifdef __cplusplus
extern "C" {
# endif
struct ffly_file* ffly_fopen(char*, int, mode_t, ffly_err_t*);
ffly_err_t ffly_fcreat(char*, mode_t);
ffly_err_t ffly_fstat(char*, struct ffly_stat*);
ffly_err_t ffly_fwrite(struct ffly_file*, void*, mdl_uint_t);
ffly_off_t ffly_fseek(struct ffly_file*, ffly_off_t, int);
ffly_err_t ffly_fread(struct ffly_file*, void*, mdl_uint_t);
ffly_err_t ffly_fclose(struct ffly_file*);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__file__h*/
