# include "dk.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
struct task {
	void(*func)(void);
	struct task *next;
};


static struct task *top = NULL;
void ffly_grdk_add(void(*__func)(void)) {
	struct task *ts;
	ts = (struct task*)__ffly_mem_alloc(sizeof(struct task));

	ts->func = __func;
	ts->next = top;
	top = ts;
}

void ffly_grdk_run(void) {
	struct task *cur;
	cur = top;
	while(cur != NULL) {
		cur->func();
		cur = cur->next;
	}
}
