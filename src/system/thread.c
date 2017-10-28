# define _GNU_SOURCE
# include <unistd.h>
# include "thread.h"
# include <signal.h>
# include <sys/types.h>
# include <errno.h>
# include "io.h"
# include <sys/wait.h>
# include "../types/bool_t.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_realloc.h"
# include "../memory/mem_free.h"
# include "../types/off_t.h"
# include "mutex.h"
# include <sched.h>
# include "atomic.h"
# include "../types/byte_t.h"
# include "../types/flag_t.h"
# define MAX_THREADS 20
# define PAGE_SIZE 4
# define UU_PAGE_SIZE 26
# define DSS 0xFFF
struct ffly_thread {
	pid_t pid;
	ffly_tid_t tid;
	ffly_bool_t alive, ok;
	ffly_byte_t *sp;
	void*(*entry_point)(void*);
	void *arg_p;
};

static struct ffly_thread **threads = NULL;
mdl_uint_t static page_c = 0;
ffly_off_t static off = 0;
ffly_atomic_uint_t active_threads = 0;

struct {
	ffly_tid_t *p;
	mdl_uint_t page_c;
	ffly_off_t off;
} uu_ids = {.p = NULL, .page_c = 0, .off = 0};
ffly_mutex_t static mutex = FFLY_MUTEX_INIT;

static struct ffly_thread *get_thr(ffly_tid_t __tid) {return *(threads+__tid);}

mdl_uint_t no_threads() {return off-uu_ids.off;}

ffly_err_t static ffly_thread_del(ffly_tid_t __tid) {
	ffly_mutex_lock(&mutex);
	if (uu_ids.off >= uu_ids.page_c*UU_PAGE_SIZE) {
		if (!uu_ids.p) {
			if ((uu_ids.p = (ffly_tid_t*)__ffly_mem_alloc(((++uu_ids.page_c)*UU_PAGE_SIZE)*sizeof(ffly_tid_t))) == NULL) {
				ffly_printf(stderr, "thread: failed to allocate memory for unused thread ids to be stored.\n");
				return FFLY_FAILURE;
			}
		} else {
			if ((uu_ids.p = (ffly_tid_t*)__ffly_mem_realloc(uu_ids.p, ((++uu_ids.page_c)*UU_PAGE_SIZE)*sizeof(ffly_id_t))) == NULL) {
				ffly_printf(stderr, "thread: failed to realloc memory space for unused thread ids.\n");
				return FFLY_FAILURE;
			}
		}
	}

	*(uu_ids.p+(uu_ids.off++)) = __tid;
	ffly_mutex_unlock(&mutex);
	return FFLY_SUCCESS;
}

ffly_bool_t ffly_thread_alive(ffly_tid_t __tid) {
	return (get_thr(__tid)->alive||!get_thr(__tid)->ok);
}

ffly_bool_t ffly_thread_dead(ffly_tid_t __tid) {
	return !get_thr(__tid)->alive;
}

int static ffly_thr_proxy(void *__arg_p) {
	struct ffly_thread *thr = (struct ffly_thread*)__arg_p;
	ffly_atomic_incr(&active_threads);
	thr->pid = getpid();
	ffly_printf(stdout, "pid: %ld, no: %lu\n", thr->pid, active_threads);
	thr->alive = 1;

	thr->ok = 1;
	thr->entry_point(thr->arg_p);
//	ffly_thread_del(thr->tid);

	thr->alive = 0;
	ffly_atomic_decr(&active_threads);
	ffly_printf(stdout, "no: %lu\n", active_threads);
	fflush(stdout);
//	if (kill(thr->pid, SIGKILL) < 0) {
//		ffly_printf(stderr, "thread: failed to kill, errno{%d}\n", errno);
//	}
//	_wait:
//	goto _wait;
	return 0;
}

ffly_err_t ffly_thread_kill(ffly_tid_t __tid) {
	struct ffly_thread *thr = get_thr(__tid);
	if (kill(thr->pid, SIGKILL) == -1) {
		ffly_printf(stderr, "thread, failed to kill, errno{%d}\n", errno);
		return FFLY_FAILURE;
	}

	while(!thr->ok);
	if (waitpid(thr->pid, NULL, 0) < 0) {
		ffly_printf(stderr, "thread, waidpid failure, errno: %d.\n", errno);
	}
	return ffly_thread_del(__tid);
}

ffly_err_t ffly_thread_create(ffly_tid_t *__tid, void*(*__p)(void*), void *__arg_p) {
	if (no_threads() == MAX_THREADS) {
		ffly_printf(stdout, "thread: only %u threads are allowed.\n", MAX_THREADS);
		return FFLY_FAILURE;
	}

	ffly_mutex_lock(&mutex);
	mdl_u8_t reused_tid = 0;
	if ((reused_tid = (uu_ids.off > 0))) {
		*__tid = *(uu_ids.p+(--uu_ids.off));
		if (uu_ids.off < ((uu_ids.page_c-1)*UU_PAGE_SIZE) && uu_ids.page_c > 1) {
			if ((uu_ids.p = (ffly_tid_t*)__ffly_mem_realloc(uu_ids.p, ((--uu_ids.page_c)*UU_PAGE_SIZE)*sizeof(ffly_id_t))) == NULL) {
				ffly_printf(stderr, "thread: failed to realloc memory space for unused thread ids.\n");
				return FFLY_FAILURE;
			}
		}
		ffly_mutex_unlock(&mutex);
		goto _exec;
	}
	ffly_mutex_unlock(&mutex);

	*__tid = off++;
	goto _alloc;

	_exec:
{
	struct ffly_thread *thr = get_thr(*__tid);
	thr->alive = 0;
	thr->ok = 0;
	if (reused_tid) {
		if (thr->sp == NULL)
			thr->sp = __ffly_mem_alloc(DSS);

		thr->arg_p = __arg_p;
		thr->entry_point = __p;
		waitpid(thr->pid, NULL, 0);
		printf("--------------------\n");
	} else {
		*thr = (struct ffly_thread) {
			.tid = *__tid,
			.sp = __ffly_mem_alloc(DSS),
			.arg_p = __arg_p,
			.entry_point = __p
		};
	}

	pid_t pid;
	if ((pid = clone(&ffly_thr_proxy, thr->sp+DSS, CLONE_VM|CLONE_SIGHAND|CLONE_FILES|CLONE_FS|SIGCHLD, (void*)thr)) == -1) {
		ffly_printf(stderr, "thread: failed.\n");
		return FFLY_FAILURE;
	}
//	waitpid(pid, NULL, 0);
}
	return FFLY_SUCCESS;

	_alloc:
	if (off > page_c*PAGE_SIZE) {
		if (!threads) {
			if ((threads = (struct ffly_thread**)__ffly_mem_alloc(((++page_c)*PAGE_SIZE)*sizeof(struct ffly_thread*))) == NULL) {
				ffly_printf(stderr, "thread: failed to alloc memory, errno{%d}, ffly_errno{%u}\n", errno, ffly_errno);
				goto _err;
			}
		} else {
			if ((threads = (struct ffly_thread**)__ffly_mem_realloc(threads, ((++page_c)*PAGE_SIZE)*sizeof(struct ffly_thread*))) == NULL) {
				ffly_printf(stderr, "thread: failed to realloc memory, errno{%d}, ffly_errno{%u}\n", errno, ffly_errno);
				goto _err;
			}
		}

		struct ffly_thread **begin = threads+((page_c-1)*PAGE_SIZE);
		struct ffly_thread **itr = begin;
		while(itr != begin+PAGE_SIZE) {
			*(itr++) = (struct ffly_thread*)__ffly_mem_alloc(sizeof(struct ffly_thread));
			//printf("----> %u\n", itr-begin);
		}
	}

	goto _exec;
	_err:
	return FFLY_FAILURE;
}

ffly_err_t ffly_thread_cleanup() {
	while(active_threads != 0);
	struct ffly_thread **itr = threads;
	//printf("--- stage 1.\n");
	while(itr != threads+off) {
		if ((*itr)->alive || !(*itr)->ok) {
			while(!(*itr)->ok);
			ffly_thread_kill((*itr)->tid);
			waitpid((*itr)->pid, NULL, 0);
		}
		if ((*itr)->sp != NULL)
			__ffly_mem_free((*itr)->sp);
		__ffly_mem_free(*(itr++));
	}

	//printf("--- stage 2.\n");
	if (threads != NULL) {
		__ffly_mem_free(threads);
		threads = NULL;
	}

	//printf("--- stage 3.\n");
	if (uu_ids.p != NULL) {
		__ffly_mem_free(uu_ids.p);
		uu_ids.p = NULL;
	}
	uu_ids.off = uu_ids.page_c = 0;
	off = page_c = 0;
}
