# include "cache.h"
# include "system/errno.h"
# include "memory/mem_free.h"
# include "memory/mem_alloc.h"
# include "ffly_def.h"
# include "clock.h"
# include "system/sched.h"
# include "system/io.h"
struct record {
	void *p;
	struct record *prev, *next;
	struct record *fd;
	ff_u64_t creation;
	ff_u32_t death;
	void *arg_p;
	ff_u32_t id;
	ff_i8_t(*perish)(void*, void*);
};

typedef struct record* recordp;
typedef recordp* recordpp;
typedef recordpp* recordppp;
ff_uint_t static max;
recordp static top = NULL;
recordp static records = NULL;
recordp static fresh;

recordp static bin = NULL;
void ffly_set_cache_dir(char *__dir) {
}

# define get_record(__id) \
	(records+(__id))

ff_err_t static __cache_del(recordp);
ff_i8_t static
update(void *__arg) {
	recordp cur = top, bk;
	while(cur != NULL) {
		bk = cur;
		cur = cur->next;
		ffly_printf("[%u] cache, creation: %u, death: %u\n", bk-records, bk->creation, bk->death);
		if (clock>=bk->creation+bk->death) {
			if (bk->perish != NULL) {
				/*
					-1 - perish was stoped
				*/
				if (bk->perish(bk->arg_p, bk->p) == -1) {
					continue;
				}
			}
			__cache_del(bk);
		}
	}
	return -1;
}

ff_err_t ffly_cache_prepare(ff_uint_t __max) {
	records = (recordp)__ffly_mem_alloc(__max*sizeof(struct record));
	fresh = records; 
	max = __max;
	ffly_schedule(update, NULL, 0);
}

void static
delink(recordp __rec) {
	if (__rec == top) {
		if ((top = __rec->next) != NULL)
			top->prev = NULL;
		return;
	}
	if (__rec->prev != NULL)
		__rec->prev->next = __rec->next;
	if (__rec->next != NULL)
		__rec->next->prev = __rec->prev; 
}

ff_err_t ffly_cache_put(ff_cache_t *__id, void *__p, ff_u32_t __death, ff_i8_t(*__perish)(void*, void*), void *__arg_p) {
	if (fresh>=records+max) {
		return FFLY_FAILURE;
	}

	recordp rec;
	if (bin != NULL) {
		rec = bin;
		bin = bin->fd;
	} else
		rec = fresh++;

	*__id = rec-records;
	if (top != NULL)
		top->prev = rec;
	*rec = (struct record) {
		.id = *__id,
		.p = __p,
		.prev = NULL,
		.next = top,
		.death = __death,
		.creation = clock,
		.perish = __perish,
		.arg_p = __arg_p
	};
	top = rec;
	return FFLY_SUCCESS;
}

void* ffly_cache_get(ff_cache_t __id, ff_err_t *__err) {
	*__err = FFLY_SUCCESS;
}

ff_err_t ffly_cache_del(ff_cache_t __id) {

}

ff_err_t 
__cache_del(recordp __rec) {
	ffly_fprintf(ffly_log, "[%u] cache obliteration.\n", __rec->id);
	delink(__rec);
	if (__rec == fresh-1) {
		fresh--;
		return FFLY_SUCCESS;
	}
	
	__rec->fd = NULL;
	if (!bin)
		bin = __rec;
	else {
		__rec->fd = bin;
		bin = __rec;
	}
	return FFLY_SUCCESS;
}


ff_err_t ffly_cache_cleanup(void) {
	__ffly_mem_free(records);
}
/*
# include <stdio.h>
int main() {
	ffly_cache_prepare(20);
	ff_id_t id;
	while(1) {
	ffly_cache_put(&id, (void*)123);
	ffly_cache_del(id);
	}
}*/
