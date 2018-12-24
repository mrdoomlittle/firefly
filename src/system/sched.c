# include "sched.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# include "io.h"
ff_mlock_t static lock = FFLY_MUTEX_INIT;

ff_u64_t static clock = 0;

sched_entityp static top = NULL;
sched_entityp static end = NULL;
#define PAGE_SHIFT 3
#define PAGE_SIZE (1<<PAGE_SHIFT)

sched_entityp static *entities = NULL;

ff_uint_t static page_c = 0;
ff_u64_t static off = 0;

sched_entityp static dead = NULL;

#define get_entity(__id) \
	((*(entities+(__id>>PAGE_SHIFT)))+(__id&(0xffffffffffffffff-(64-PAGE_SHIFT))))

#define sched_lock ffly_mutex_lock(&lock)
#define sched_unlock ffly_mutex_unlock(&lock)

#define entity_lock(__ent) \
	ffly_mutex_lock(&(__ent)->lock)
#define entity_unlock(__ent) \
	ffly_mutex_unlock(&(__ent)->lock)

void static
delink(sched_entityp __ent) {
	*__ent->bk = __ent->fd;
	if (__ent->fd != NULL)
		__ent->fd->bk = __ent->bk;
}

ff_u32_t ffly_schedule(ff_i8_t(*__func)(long long), long long __arg, ff_u64_t __interval) {
	sched_lock;
	ff_u32_t ret;
	ff_uint_t page = off>>PAGE_SHIFT;
	ff_uint_t pg_off;
	sched_entityp ent;
	if (dead != NULL) {
		ent = dead;
		dead = dead->fd;
		ret = ent->id;
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
	pg_off = (ret = off++)-(page*PAGE_SIZE);
	ent = (*(entities+page))+pg_off;
_dead:
	ffly_fprintf(ffly_log, "sched, interval: %u, id: %u\n", __interval, ret);
	if (!top)
		top = ent;
	ent->inuse = 0;
	ent->prev = end;
	ent->next = NULL;
	ent->lock = FFLY_MUTEX_INIT;
	ent->func = __func;
	ent->arg = __arg;
	ent->interval = __interval;
	ent->elapsed = 0;
	ent->fd = NULL;
	if (end != NULL)
		end->next = ent;
	end = ent;
	sched_unlock;
	return (ent->id = ret);
}

void static 
deatach(sched_entityp __ent) {
	ffly_fprintf(ffly_log, "sched-deatach, id: %u\n", __ent->id);
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
		} else
			top = NULL;
		return;
	}

	if (__ent->prev != NULL)
		__ent->prev->next = __ent->next;
	if (__ent->next != NULL)
		__ent->next->prev = __ent->prev;
}

void static
remove(sched_entityp __ent) {
	sched_lock;
	deatach(__ent);
	__ent->fd = dead;
	__ent->bk = &dead;
	if (dead != NULL)
		dead->bk = &__ent->fd;
	dead = __ent;
	__ent->inuse = -1;
	sched_unlock;
}

void ffly_sched_rm(ff_u32_t __id) {
	sched_lock;
	if (__id == off-1) {
		deatach(get_entity(__id));
		off--;
		sched_entityp p;
		ff_uint_t page, pg_off;
	_again:
		if (off>0) {
			off--;
			page = off>>PAGE_SHIFT;
			pg_off = off-(page*PAGE_SIZE);
			p = (*(entities+page))+pg_off;

			if (p->inuse == -1) {
				delink(p);
				ffly_fprintf(ffly_log, "sched, upper free.\n");
				goto _again;
			} else
				ffly_fprintf(ffly_log, "sched upper inuse.\n");
		}

		off++;
		page = off>>PAGE_SHIFT;
		if (page < page_c-1 && page_c>1) {
			sched_entityp *p = entities+(page+1);
			sched_entityp *end = entities+page_c;
			while(p != end)
				__ffly_mem_free(*(p++));
			page_c = page+1;
			entities = (sched_entityp*)__ffly_mem_realloc(entities, page_c*sizeof(sched_entityp));
		}
		sched_unlock;
		return;
	}
	sched_unlock;

	remove(get_entity(__id));
}

void ffly_sched_clock_tick(ff_u32_t __delta) {
	clock+=__delta;
}

#define set_flag(__flag) \
	__asm__("mov %1, %%eax\n" \
			"lock orb %%al, %0\n" : "=m"(flags) : "r"(__flag) : "eax");
#define clr_flag(__flag) \
	__asm__("mov %1, %%eax\n" \
			"lock xorb %%al, %0\n" : "=m"(flags) : "r"(__flag) : "eax");
#define is_flag(__flags, __flag) \
	((__flags&__flag)==__flag)

#define STOP 0x01
#define INIT 0x02
#define OKAY 0x04
ff_i8_t static flags = 0x00;
ff_u64_t static last_time = 0;

void ffly_scheduler_tick(void) {	
	if (!is_flag(flags, INIT)) {
		return;
	}

	if (is_flag(flags, STOP)) {
		if (!is_flag(flags, OKAY)) {
			set_flag(OKAY);
		}
		return;
	}
	
	sched_entityp cur = top, ent;
	while(cur != NULL) {
		ent = cur;
		cur = cur->next;
		if ((ent->elapsed+=clock-last_time) >= ent->interval) {
			ent->elapsed = 0;
			if (!ent->func(ent->arg)) {
				remove(ent);
			}
		}
	}
	last_time = clock;
}

# include "../corrode.h"
# include "io.h"

void static
corrode(void *__arg) {
	ffly_printf("scheduler-corrode.\n");
	ffly_scheduler_de_init();
}

void ffly_scheduler_init(ff_u8_t __flags) {	
	set_flag(INIT);
	ffly_fprintf(ffly_log, "sched init.\n");
	if (is_flag(__flags, SCHED_CORRODE)) {
		ffly_corrode(corrode, NULL);
	}
}

void ffly_scheduler_de_init(void) {
	ffly_fprintf(ffly_log, "sched de-init.\n");
	set_flag(STOP);
	// decomment ??????
//	while(!is_flag(flags, OKAY));
	sched_entityp *cur = entities;
	sched_entityp *end = cur+page_c;
	while(cur != end)
		__ffly_mem_free(*(cur++));
	if (entities != NULL)
		__ffly_mem_free(entities);
}
