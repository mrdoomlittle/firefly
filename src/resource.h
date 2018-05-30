# ifndef __ffly__resource__h
# define __ffly__resource__h
# include "ffint.h"
# include "types.h"
# include "linux/stat.h"
//# define FFLY_RS_
# define RESOURCE_DIR "resource"
# define FFLY_RES_CACHED 0x1
struct ffly_rs_hdr {
	char ident[4];
};

typedef struct ffly_resource {
	ff_u8_t *p;
	ff_u8_t flags;

	// cleanup
	struct ffly_resource *fd;
	ff_uint_t id;
	ff_id_t cache;
} *ffly_resourcep;

typedef ff_uint_t* ff_rid_t;

void ffly_resource_creat(ff_rid_t*);
void ffly_resource_del(ff_rid_t);
void ffly_resource_cleanup(void);
ffly_resourcep ffly_resource_get(ff_rid_t);
ff_err_t ffly_resource_move(char const*);
void ffly_resource_load_bin(ff_rid_t, char const*);
# endif /*__ffly__resource__h*/
