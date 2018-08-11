# ifndef __ffly__resource__h
# define __ffly__resource__h
# include "ffint.h"
# include "types.h"
# include "linux/stat.h"
//# define FFLY_RS_
# define RESOURCE_DIR "resource"
# define FFLY_RES_CACHED 0x01
# define FFLY_RES_OPEN 0x02
struct ffly_rs_hdr {
	char ident[4];
};

typedef struct ffly_rs_loaf {
	ff_cache_t cache;
	ff_u16_t lock;
	ff_u8_t flags;
	void *p;
	ff_u32_t off;
	struct ffly_rs_loaf **bk, *link;
	ff_u64_t creation;
	ff_u32_t death;
} *ffly_rs_loafp;

typedef struct ffly_resource {
	ffly_rs_loafp loafs;
	ff_u8_t flags;

	// cleanup
	struct ffly_resource *fd;
	ff_u8_t c;
	void *r;
} *ffly_resourcep;

enum {
	_rs_file
};

void ffly_resource_load(ff_rs_t, ff_uint_t, void**);
void ffly_resource_creat(ff_rs_t*);
void ffly_resource_del(ff_rs_t);
void ffly_resource_open(ff_rs_t);
void ffly_resource_close(ff_rs_t);
void ffly_resource_cleanup(void);
ffly_resourcep ffly_resource_get(ff_rs_t);
ff_err_t ffly_resource_move(char const*);
void ffly_resource_load_bin(ff_rs_t, char const*);
# endif /*__ffly__resource__h*/
