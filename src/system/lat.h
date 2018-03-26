# ifndef __ffly__lat__h
# define __ffly__lat__h
# include <mdlint.h>
# define ff_lat struct ffly_lat
# ifdef __ffly_lat_internal
typedef struct record {
	char ident[3];
	struct record *next, *fd;
	mdl_u64_t key;
	void *p;
} *recordp;

typedef struct pod {
	void *p;
} *podp;

# endif
typedef struct ffly_lat {
# ifdef __ffly_lat_internal
	podp p;
	recordp head;
# else
	mdl_u8_t pad[16];
# endif
} *ffly_latp;

void* ffly_lat_head(ffly_latp);
void ffly_lat_fd(void**);
void* ffly_lat_getp(void*);
void ffly_lat_prepare(ffly_latp);
void ffly_lat_put(ffly_latp, mdl_u64_t, void*);
void* ffly_lat_get(ffly_latp, mdl_u64_t);
void ffly_lat_free(ffly_latp);
# endif /*__ffly__lat__h*/
