# ifndef __ffly__mal__track__h
# define __ffly__mal__track__h
# include "../ffint.h"
# include "../types.h"
# include "mutex.h"
# ifdef __cplusplus
extern "C" {
# endif

struct ffly_mal_track {
	void **table;
	ff_bool_t inited;
	ff_mlock_t lock;
};

extern struct ffly_mal_track __ffly_mal_track__;
# ifdef __ffly_debug
void ffly_mal_track_dump(struct ffly_mal_track*);
# endif
ff_err_t ffly_mal_track_update(struct ffly_mal_track*, void*, void*);
ff_err_t ffly_mal_track_init(struct ffly_mal_track*);
ff_err_t ffly_mal_track_de_init(struct ffly_mal_track*);
ff_err_t ffly_mal_track_alloc(struct ffly_mal_track*, void*);
ff_err_t ffly_mal_track_free(struct ffly_mal_track*, void*, ff_u8_t);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__mal__track__h*/
