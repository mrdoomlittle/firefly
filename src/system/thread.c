# define _GNU_SOURCE
# include <unistd.h>
# include "thread.h"
# include <signal.h>
# include <sys/types.h>
# include <errno.h>
# include "io.h"
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
# define DSS 32000
struct ffly_thread {
	pid_t pid;
	ffly_tid_t tid;
	ffly_bool_t alive;
	ffly_byte_t *sp;
	void*(*entry_point)(void*);
	void *arg_p;
};

static struct ffly_thread *threads = NULL;
mdl_uint_t static page_c = 0;
ffly_off_t static off = 0;
ffly_atomic_uint_t active_threads = 0;

struct {
	ffly_tid_t *p;
	mdl_uint_t page_c;
	ffly_off_t off;
} uu_ids = {.p = NULL, .page_c = 0, .off = 0};
ffly_mutex_t static mutex = 0;

static struct ffly_thread *get_thr(ffly_tid_t __tid) {return threads+__tid;}

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

int static ffly_thr_proxy(void *__arg_p) {
	struct ffly_thread *thr = (struct ffly_thread*)__arg_p;
	ffly_atomic_incr(&active_threads);
	thr->pid = getpid();
	thr->alive = 1;

	thr->entry_point(thr->arg_p);

	thr->alive = 0;
	ffly_thread_del(thr->tid);

	ffly_atomic_decr(&active_threads);
	if (kill(thr->pid, SIGKILL) == -1) {
		ffly_printf(stderr, "thread: failed to kill, errno{%d}\n", errno);
	}

	return 0;
}

ffly_err_t ffly_thread_kill(ffly_tid_t __tid) {
	struct ffly_thread *thr = get_thr(__tid);
	if (kill(thr->pid, SIGKILL) == -1) {
		ffly_printf(stderr, "thread: failed to kill, errno{%d}\n", errno);
		return FFLY_FAILURE;
	}

	return ffly_thread_del(__tid);
}

ffly_err_t ffly_thread_create(ffly_tid_t *__tid, void*(*__p)(void*), void *__arg_p) {
	if (no_threads() == MAX_THREADS) {
		ffly_printf(stdout, "thread: only %u threads are allowed.\n", MAX_THREADS);
		return FFLY_FAILURE;
	}

	mdl_u8_t reused_tid;
	if ((reused_tid = (uu_ids.off > 0))) {
		*__tid = *(uu_ids.p+(--uu_ids.off));
		if (uu_ids.off < ((uu_ids.page_c-1)*UU_PAGE_SIZE) && uu_ids.page_c > 1) {
			if ((uu_ids.p = (ffly_tid_t*)__ffly_mem_realloc(uu_ids.p, ((--uu_ids.page_c)*UU_PAGE_SIZE)*sizeof(ffly_id_t))) == NULL) {
				ffly_printf(stderr, "thread: failed to realloc memory space for unused thread ids.\n");
				return FFLY_FAILURE;
			}
		}
		goto _exec;
	}

	*__tid = off++;
	goto _alloc;

	_exec:
{
	struct ffly_thread *thr = get_thr(*__tid);
	if (reused_tid) {
		if (thr->sp == NULL)
			thr->sp = __ffly_mem_alloc(DSS);

		thr->arg_p = __arg_p;
		thr->entry_point = __p;
	} else {
		*thr = (struct ffly_thread) {
			.tid = *__tid,
			.sp = __ffly_mem_alloc(DSS),
			.arg_p = __arg_p,
			.entry_point = __p
		};
	}

	if (clone(&ffly_thr_proxy, thr->sp+DSS, CLONE_VM, (void*)thr) == -1) {
		ffly_printf(stderr, "thread: failed.\n");
		return FFLY_FAILURE;
	}
}
	return FFLY_SUCCESS;

	_alloc:
	if (off > page_c*PAGE_SIZE) {
		if (!threads) {
			if ((threads = (struct ffly_thread*)__ffly_mem_alloc(((++page_c)*PAGE_SIZE)*sizeof(struct ffly_thread))) == NULL) {
				ffly_printf(stderr, "thread: failed to alloc memory, errno{%d}, ffly_errno{%u}\n", errno, ffly_errno);
				goto _err;
			}
		} else {
			if ((threads = (struct ffly_thread*)__ffly_mem_realloc(threads, ((++page_c)*PAGE_SIZE)*sizeof(struct ffly_thread))) == NULL) {
				ffly_printf(stderr, "thread: failed to realloc memory, errno{%d}, ffly_errno{%u}\n", errno, ffly_errno);
				goto _err;
			}
		}
	}

	goto _exec;
	_err:
	return FFLY_FAILURE;
}

ffly_err_t ffly_thread_cleanup() {
	while(no_threads() != 0 || active_threads != 0);
	struct ffly_thread *itr = threads;
	while(itr != threads+off) {
		__ffly_mem_free(itr->sp);
		itr++;
	}

	if (threads != NULL) {
		__ffly_mem_free(threads);
		threads = NULL;
	}

	if (uu_ids.p != NULL) {
		__ffly_mem_free(uu_ids.p);
		uu_ids.p = NULL;
	}
	uu_ids.off = uu_ids.page_c = 0;
	off = page_c = 0;
}
