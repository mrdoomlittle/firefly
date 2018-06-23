# include "servant.h"
# include "errno.h"
# include "flags.h"
# include "io.h"
# include "nanosleep.h"
void static*
handle(void *__arg_p) {
	ffly_fprintf(ffly_log, "servant has awoken.\n");
	ffly_servantp arg = (ffly_servantp)__arg_p;
	struct timespec beg, end;
	do {
		clock_gettime(CLOCK_MONOTONIC, &end);
		/*
			we dont need to know the exact latency

		*/
		arg->latency = ((end.tv_sec-beg.tv_sec)>>10)+((end.tv_nsec-beg.tv_nsec)>>20);
		clock_gettime(CLOCK_MONOTONIC, &beg);

		ffly_taskp cur, bk;
	_again:
		if (!(cur = arg->top)) {
			ffly_nanosleep(0, 100);
			if (ffly_is_flag(arg->flags, FF_SERVANT_KILL))
				break;
			goto _again;
		}

		ffly_mutex_lock(&arg->lock);
		while(cur != NULL) {
			bk = cur;
			cur = cur->next;
			if (!bk->func(bk->arg_p)) {
				ffly_mutex_unlock(&arg->lock);
				ffly_servant_task_rm(arg, bk);
				//__ffly_mem_free(bk);
				ffly_mutex_lock(&arg->lock);
			}
		}
		ffly_mutex_unlock(&arg->lock);
	//	ffly_nanosleep(1, 0);
	} while(!ffly_is_flag(arg->flags, FF_SERVANT_KILL));
	ffly_add_flag(arg->flags, FF_SERVANT_DEAD);
	ffly_fprintf(ffly_log, "servant has died.\n");
}

ffly_servantp ffly_servant(ff_err_t *__err) { 
	*__err = FFLY_SUCCESS;
	ffly_servantp p = (ffly_servantp)__ffly_mem_alloc(sizeof(struct ffly_servant));
	p->lock = FFLY_MUTEX_INIT;
	p->top = NULL;
	p->flags = 0x0;
	p->latency = 0;
	*__err = ffly_thread_create(&p->tid, handle, p);
	return p;
}

void ffly_servant_task_add(ffly_servantp __serv, ffly_taskp __task) {
	ffly_mutex_lock(&__serv->lock);
	__task->prev = NULL;
	__task->next = __serv->top;
	__serv->top = __task;
	ffly_mutex_unlock(&__serv->lock);
}

void ffly_servant_task_rm(ffly_servantp __serv, ffly_taskp __task) {
	ffly_mutex_lock(&__serv->lock);
	if (__serv->top == __task) {
		if ((__serv->top = __task->next) != NULL)
			__serv->top->prev = NULL;
		goto _out;
	}

	if (__task->prev != NULL)
		__task->prev->next = __task->next;
	if (__task->next != NULL)
		__task->next->prev = __task->prev;
_out:
	ffly_mutex_unlock(&__serv->lock);
}

void ffly_servant_kill(ffly_servantp __servant) {
	__servant->flags |= FF_SERVANT_KILL;
	ffly_thread_wait(__servant->tid);
	__ffly_mem_free(__servant);
}
