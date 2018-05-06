# ifndef __ffly__sched__h
# define __ffly__sched__h
# include "../ffint.h"

// 1 millisecond
# define SCHED_TIME_PERIOD 0.001

typedef struct sched_entity {
	ff_u64_t interval;
	ff_u64_t elapsed;
	void(*func)(void*);
	void *arg_p;
	struct sched_entity *prev, *next;
} *sched_entityp;

void ffly_schedule(void(*)(void*), void*, ff_u64_t);
void ffly_sched_clock_tick(ff_u64_t);
void ffly_scheduler_tick();
void ffly_scheduler_init();
void ffly_scheduler_de_init();
# endif /*__ffly__sched__h*/
