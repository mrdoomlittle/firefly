# ifndef __ffly__file__h
# define __ffly__file__h
# include "../types/size_t.h"
# include <sys/types.h>
struct ffly_file {
	int fd;
};

struct ffly_stat {
	ffly_size_t size;
};

# define FFLY_FILE struct ffly_file
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
