# ifndef __ffly__file__h
# define __ffly__file__h
# include "../types/size_t.h"
# include <sys/types.h>
# include <fcntl.h>
struct ffly_file {
	int d;
};


struct ffly_stat {
	ffly_size_t size;
};

# define FF_O_CREATE O_CREAT
# define FF_S_IRWXU S_IRWXU
# define FF_S_IRUSR S_IRUSR
# define FF_S_IWUSR S_IWUSR
# define FF_S_IXUSR S_IXUSR
# define FF_FILE struct ffly_file
# ifdef __cplusplus
extern "C" {
# endif
struct ffly_file* ffly_fopen(char*, int, mode_t);
void ffly_fcreat(char*, mode_t);
void ffly_fstat(char*, struct ffly_stat*);
void ffly_fwrite(struct ffly_file*, void*, ffly_size_t);
void ffly_fread(struct ffly_file*, void*, ffly_size_t);
void ffly_fclose(struct ffly_file*);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__file__h*/
