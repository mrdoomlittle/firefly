# ifndef __ffly__sched__h
# define __ffly__sched__h
# include "../ffint.h"
# include "mutex.h"
// 1 millisecond
# define SCHED_TIME_PERIOD 0.001
# define SCHED_CORRODE 0x1
typedef struct sched_entity {
	ff_u64_t interval;
	ff_u64_t elapsed;
	ff_i8_t(*func)(void*);
	void *arg_p;
	struct sched_entity *prev, *next;
	struct sched_entity *fd, **bk;
	ff_mlock_t lock;
	ff_u32_t id;
	ff_i8_t inuse;
} *sched_entityp;

void ffly_sched_rm(ff_u32_t);
ff_u32_t ffly_schedule(ff_i8_t(*)(void*), void*, ff_u64_t);
void ffly_sched_clock_tick(ff_u64_t);
void ffly_scheduler_tick(void);
void ffly_scheduler_init(ff_u8_t);
void ffly_scheduler_de_init(void);
# endif /*__ffly__sched__h*/
