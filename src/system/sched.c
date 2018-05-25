# include "sched.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
ff_u64_t static clock = 0;
sched_entityp static top = NULL;
sched_entityp static end = NULL;
# define PAGE_SHIFT 3
# define PAGE_SIZE (1<<PAGE_SHIFT)

sched_entityp static *entities = NULL;

ff_uint_t static page_c = 0;
ff_u64_t static off = 0;

sched_entityp static dead = NULL;

# define get_entity(__id) \
	((*(entities+(__id&0xfff)))+((__id>>12)&0xfffff))

ff_u32_t ffly_schedule(ff_i8_t(*__func)(void*), void *__arg_p, ff_u64_t __interval) {
	ff_uint_t page = off>>PAGE_SHIFT;
	ff_uint_t pg_off;
	sched_entityp ent;
	if (dead != NULL) {
		ent = dead;
		dead = dead->fd;
		goto _dead;
	}
	if (!entities) {
		entities = (sched_entityp*)__ffly_mem_alloc(sizeof(sched_entityp));
		page_c++;
	} else {
		if (page>page_c-1)
			entities = (sched_entityp*)__ffly_mem_realloc(entities, (++page_c)*sizeof(sched_entityp));
		else
			goto _sk;
	}
	*(entities+page) = (sched_entityp)__ffly_mem_alloc(PAGE_SIZE*sizeof(struct sched_entity));
_sk:
	pg_off = (off++)-(page*PAGE_SIZE);
	ent = (*(entities+page))+pg_off;
_dead:
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
	ent->fd = NULL;
	return (page&0xfff)|((pg_off&0xfffff)<<12);
}

void static 
deatach(sched_entityp __ent) {
	if (__ent == top) {
		if ((top = __ent->next) != NULL) {
			top->prev = NULL;
		} else
			end = NULL;
		return;
	}

	if (__ent == end) {
		if ((end = __ent->prev) != NULL) {
			end->next = NULL;
		}
		return;
	}

	if (__ent->prev != NULL)
		__ent->prev->next = __ent->next;
	if (__ent->next != NULL)
		__ent->next->prev = __ent->prev;
}

void static
remove(sched_entityp __ent) {
	deatach(__ent);
	__ent->fd = dead;
	dead = __ent;
}

void ffly_sched_rm(ff_u32_t __id) {
	if (((__id&0xfff)*PAGE_SIZE)+((__id>>12)&0xfffff) == off-1) {
		deatach(get_entity(__id));
		off--;
		ff_uint_t page = off>>PAGE_SHIFT;
		if (page < page_c-1 && page_c>1) {
			__ffly_mem_free(*(entities+(page+1)));
			entities = (sched_entityp*)__ffly_mem_realloc(entities, (--page_c)*sizeof(sched_entityp));
		}
	}

	remove(get_entity(__id));
}

void ffly_sched_clock_tick(ff_u64_t __delta) {
	clock+=__delta;
}

ff_u64_t static last_time = 0;
void ffly_scheduler_tick() {
	
	sched_entityp cur = top, ent;
	while(cur != NULL) {
		ent = cur;
		cur = cur->next;
		if ((ent->elapsed+=clock-last_time) >= ent->interval) {
			ent->elapsed = 0;
			if (!ent->func(ent->arg_p)) {
				remove(ent);
			}
		}
	}
	last_time = clock;
}

void ffly_scheduler_init() {
	
}

void ffly_scheduler_de_init() {
	sched_entityp *cur = entities;
	sched_entityp *end = cur+page_c;
	while(cur != end)
		__ffly_mem_free(*(cur++));
	if (entities != NULL)
		__ffly_mem_free(entities);
}
