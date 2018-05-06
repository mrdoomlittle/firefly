# include "sched.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
ff_u64_t static clock = 0;
sched_entityp static top = NULL;
sched_entityp static end = NULL;

void ffly_schedule(void(*__func)(void*), void *__arg_p, ff_u64_t __interval) {
	sched_entityp ent = (sched_entityp)__ffly_mem_alloc(sizeof(struct sched_entity));
	if (!top)
		top = ent;

	ent->prev = end;
	if (end != NULL)
		end->next = ent;
	end = ent;
	
	ent->func = __func;
	ent->arg_p = __arg_p;
	ent->interval = __interval;
	ent->elapsed = 0;
}

void ffly_sched_clock_tick(ff_u64_t __delta) {
	clock+=__delta;
}

ff_u64_t static last_time = 0;
void ffly_scheduler_tick() {
	
	sched_entityp cur = top;
	while(cur != NULL) {
		if ((cur->elapsed+=clock-last_time) >= cur->interval) {
			cur->elapsed = 0;
			cur->func(cur->arg_p);
		}
		cur = cur->next;
	}
	last_time = clock;
}

void ffly_scheduler_init() {
	
}

void ffly_scheduler_de_init() {
	sched_entityp cur = top, bk;
	while(cur != NULL) {
		bk = cur;
		cur = cur->next;
		__ffly_mem_free(bk);
	}
}
