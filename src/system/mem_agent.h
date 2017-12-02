# ifndef __ffly__mem__agent__h
# define __ffly__mem__agent__h
# include "../types/err_t.h"
# include <mdlint.h>
# include "../types/bool_t.h"
# include "mutex.h"
# ifdef __cplusplus
extern "C" {
# endif

struct ffly_mem_agent {
	void **table;
	ffly_bool_t inited;
	ffly_mutex_t mutex;
};

extern struct ffly_mem_agent __ffly_mem_agent__;
# ifdef __DEBUG_ENABLED
void ffly_mem_agent_dump(struct ffly_mem_agent*);
# endif
//void* ffly_mem_agent_get_nxt(struct ffly_mem_agent*);
//void ffly_mem_agent_reset(struct ffly_mem_agent*);
ffly_err_t ffly_mem_agent_update(struct ffly_mem_agent*, void*, void*);
ffly_err_t ffly_mem_agent_init(struct ffly_mem_agent*);
ffly_err_t ffly_mem_agent_de_init(struct ffly_mem_agent*);
ffly_err_t ffly_mem_agent_alloc(struct ffly_mem_agent*, void*);
ffly_err_t ffly_mem_agent_free(struct ffly_mem_agent*, void*, mdl_u8_t);
# ifdef __cplusplus
}
# endif
# endif /*__mem__tracker__h*/
