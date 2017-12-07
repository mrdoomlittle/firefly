# ifndef __ffly__mal__track__h
# define __ffly__mal__track__h
# include "../types/err_t.h"
# include <mdlint.h>
# include "../types/bool_t.h"
# include "mutex.h"
# ifdef __cplusplus
extern "C" {
# endif

struct ffly_mal_track {
	void **table;
	ffly_bool_t inited;
	ffly_mutex_t mutex;
};

extern struct ffly_mal_track __ffly_mal_track__;
# ifdef __DEBUG_ENABLED
void ffly_mal_track_dump(struct ffly_mal_track*);
# endif
//void* ffly_mal_track_get_nxt(struct ffly_mal_track*);
//void ffly_mal_track_reset(struct ffly_mal_track*);
ffly_err_t ffly_mal_track_update(struct ffly_mal_track*, void*, void*);
ffly_err_t ffly_mal_track_init(struct ffly_mal_track*);
ffly_err_t ffly_mal_track_de_init(struct ffly_mal_track*);
ffly_err_t ffly_mal_track_alloc(struct ffly_mal_track*, void*);
ffly_err_t ffly_mal_track_free(struct ffly_mal_track*, void*, mdl_u8_t);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__mal__track__h*/
