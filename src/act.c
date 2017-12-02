# include "act.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "memory/mem_realloc.h"
# include "system/io.h"
# define get_group(__act, __gid) (*(__act->groups+__gid))
void ffly_act_init(struct ffly_act *__act) {
	__act->groups = NULL;
	__act->no_groups = 0;
}

ffly_act_gid_t ffly_act_add_group(struct ffly_act *__act) {
	ffly_act_gid_t gid = __act->no_groups;
	if (!__act->no_groups) {
		__act->groups = (struct ffly_act_group**)__ffly_mem_alloc(sizeof(struct ffly_act_group*));
		__act->no_groups++;
	} else if (__act->no_groups)
		__act->groups = (struct ffly_act_group**)__ffly_mem_realloc(__act->groups, (++__act->no_groups)*sizeof(struct ffly_act_group*));
	*((get_group(__act, gid) = (struct ffly_act_group*)__ffly_mem_alloc(sizeof(struct ffly_act_group))))= (struct ffly_act_group) {
		.no_tasks = 0, .tasks = (struct ffly_act_task*)__ffly_mem_alloc(NO_TASKS*sizeof(struct ffly_act_task))
	};
	return gid;
}

void ffly_act_add_task(struct ffly_act *__act, ffly_act_gid_t __gid, void(*__call)(void*), void *__arg_p) {
	struct ffly_act_group *group = get_group(__act, __gid);
	*(group->tasks+(group->no_tasks++)) = (struct ffly_act_task) {.call = __call, .arg_p = __arg_p};
}

void ffly_act_do(struct ffly_act *__act, ffly_act_gid_t __gid) {
	struct ffly_act_group *group = get_group(__act, __gid);
	struct ffly_act_task *itr = group->tasks;
	while(itr != group->tasks+group->no_tasks) {
		ffly_printf(stdout, "running task %u, group: %u.\n", itr-group->tasks, __gid);
		itr->call(itr->arg_p);
		itr++;
	}
}

struct ffly_act __ffly_act__;



