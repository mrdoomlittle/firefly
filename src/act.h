# ifndef __ffly__act__h
# define __ffly__act__h
# define ACT_GID_INIT 0
# define ACT_GID_DE_INIT 1
# define NO_TASKS 100
# include <mdlint.h>
# include "types/err_t.h"
/*
	TODO:
		repurpose for actions in specific areas of engine
*/

typedef mdl_uint_t ffly_act_gid_t;

struct ffly_act_task {
	void(*call)(void*);
	void *arg_p;
};

struct ffly_act_group {
	mdl_uint_t no_tasks;
	struct ffly_act_task *tasks;

};

struct ffly_act {
	struct ffly_act_group **groups;
	mdl_uint_t no_groups;
};

# ifdef __cplusplus
extern "C" {
# endif
extern struct ffly_act __ffly_act__;
ffly_err_t ffly_act_init(struct ffly_act*);
ffly_act_gid_t ffly_act_add_group(struct ffly_act*);
ffly_err_t ffly_act_add_task(struct ffly_act*, ffly_act_gid_t, void(*)(void*), void*);
ffly_err_t ffly_act_do(struct ffly_act*, ffly_act_gid_t);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__act__h*/
