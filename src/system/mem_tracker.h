# ifndef __mem__tracker__h
# define __mem__tracker__h
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# include "errno.h"
# include "../types/err_t.h"
# include <errno.h>
# include <eint_t.h>
# ifdef __DEBUG_ENABLED
#	include <stdio.h>
# endif
# ifdef __cplusplus
extern "C" {
# endif

struct ffly_mem_track {
	void **mptr_lst;
	mdl_uint_t *unused_pnts;
	mdl_uint_t unused_pnts_c;
	mdl_uint_t lst_size;
	mdl_uint_t nxt_pnt;
};

extern struct ffly_mem_track __ffly_mem_track__;
# ifdef __DEBUG_ENABLED
void ffly_mem_track_dmp_ptr_lst(struct ffly_mem_track*);
# endif
void* ffly_mem_track_get_nxt(struct ffly_mem_track*);
void ffly_mem_track_reset(struct ffly_mem_track*);
void ffly_mem_track_update(struct ffly_mem_track*, void*, void*);
ffly_err_t ffly_mem_track_init(struct ffly_mem_track*);
ffly_err_t ffly_mem_track_de_init(struct ffly_mem_track*);
ffly_err_t ffly_mem_track_alloc(struct ffly_mem_track*, void*);
ffly_err_t ffly_mem_track_free(struct ffly_mem_track*, void*, mdl_u8_t);
# ifdef __cplusplus
}
# endif
# endif /*__mem__tracker__h*/
