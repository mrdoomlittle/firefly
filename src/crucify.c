# include "crucify.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "ffly_def.h"
/*
	de init


	when initing shit it will push crucify function to de init
*/

struct task {
	void(*func)(void*);
	void *arg_p;
	struct task *next;
};

static struct task *top = NULL;
void ffly_crucify(void(*__func)(void*), void *__arg_p) {
	struct task *t = (struct task*)__ffly_mem_alloc(sizeof(struct task));
	t->func = __func;
	t->arg_p = __arg_p;

	t->next = top;
	top = t;
}

void ffly_crucify_run(void) {
	struct task *cur = top, *bk;
	while(cur != NULL) {
		bk = cur;
		cur = cur->next;
		bk->func(bk->arg_p);
		__ffly_mem_free(bk);
	}
}
