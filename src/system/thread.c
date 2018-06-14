# include "thread.h"
# include "io.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_realloc.h"
# include "../memory/mem_free.h"
# include "../ctl.h"
# include "mutex.h"
# include "atomic.h"
# include "cond_lock.h"
# ifndef __ffly_no_sysconf
# define MAX_THREADS __ffly_sysconf__.max_threads
# else
# define MAX_THREADS 20
# endif
# define PAGE_SIZE 4
# define UU_PAGE_SIZE 26
# define DSS 872144
# define TLS_SIZE 8024
# include "../ffly_system.h"
# ifndef __ffly_no_sysconf
#	include "config.h"
# endif
# include "../mal.h"
typedef struct ffly_thread {
	ff_cond_lock_t lock;
	__linux_pid_t pid;
	ff_tid_t tid; // change to 'id'
	ff_bool_t alive;
	ff_i8_t exit;
	ff_byte_t *sp, *tls;
	void*(*routine)(void*);
	void *arg_p;
	ffly_potp pot;
} *ffly_threadp;

static struct ffly_thread **threads = NULL;
ff_uint_t static page_c = 0;
ff_off_t static off = 0;
ff_atomic_uint_t active_threads = 0;
ffly_potp static pot = NULL;

# include "../linux/signal.h"
# include "../linux/types.h"
# include "../linux/sched.h"
# include "../linux/unistd.h"
# include "../linux/wait.h"
# include "tls.h"

struct {
	ff_tid_t *p;
	ff_uint_t page_c;
	ff_off_t off;
} uu_ids = {
	.p = NULL,
	.page_c = 0,
	.off = 0
};

ff_mlock_t static mutex = FFLY_MUTEX_INIT;

# define get_thr(__tid) \
	(*(threads+(__tid)))

# define no_threads \
	(off-uu_ids.off)

static __thread ff_tid_t id = FFLY_TID_NULL;
ff_tid_t ffly_gettid() {
	return id;
}

__linux_pid_t ffly_thread_getpid(ff_tid_t __tid) {
	return get_thr(__tid)->pid;
}

void ffly_thread_init() {
	/* get current pot for this process
	*/
	ffly_ctl(ffly_malc, _ar_getpot, (ff_u64_t)&pot);
}

ff_err_t static
ffly_thread_del(ff_tid_t __tid) {
	// keep memory on pot 0
	ffly_mutex_lock(&mutex);
	if (uu_ids.off >= uu_ids.page_c*UU_PAGE_SIZE) {
		ffly_ctl(ffly_malc, _ar_setpot, (ff_u64_t)pot);
		if (!uu_ids.p) {
			if ((uu_ids.p = (ff_tid_t*)__ffly_mem_alloc(((++uu_ids.page_c)*UU_PAGE_SIZE)*sizeof(ff_tid_t))) == NULL) {
				ffly_fprintf(ffly_err, "thread: failed to allocate memory for unused thread ids to be stored.\n");
				goto _fail;
			}
		} else {
			if ((uu_ids.p = (ff_tid_t*)__ffly_mem_realloc(uu_ids.p, ((++uu_ids.page_c)*UU_PAGE_SIZE)*sizeof(ff_tid_t))) == NULL) {
				ffly_fprintf(ffly_err, "thread: failed to realloc memory for unused thread ids.\n");
				goto _fail;
			}
		}

		ffly_ctl(ffly_malc, _ar_unset, 0);
		goto _sk;
	_fail:
		ffly_ctl(ffly_malc, _ar_unset, 0);
		ffly_mutex_unlock(&mutex);
		return FFLY_FAILURE;
	}

_sk:
	*(uu_ids.p+(uu_ids.off++)) = __tid;
	ffly_mutex_unlock(&mutex);
	return FFLY_SUCCESS;
}

// ??? 
void static
thread_free(ff_tid_t __id) {
	struct ffly_thread *thr = get_thr(__id);
	__ffly_mem_free(thr->sp);
	thr->sp = NULL;
}

ff_bool_t ffly_thread_alive(ff_tid_t __tid) {
	return get_thr(__tid)->alive;
}

ff_bool_t ffly_thread_dead(ff_tid_t __tid) {
	return !get_thr(__tid)->alive;
}

# include "../tools/printbin.c"
void static
prox() {
	void *arg_p;
	__asm__("movq -8(%%rbp), %%rdi\n\t"
			"movq %%rdi, %0": "=m"(arg_p) : : "rdi");
	ffly_tls_init();
	ffly_process_prep();
	ffly_threadp thr = (ffly_threadp)arg_p;
	ffly_cond_lock_signal(&thr->lock);
	/*
	ff_setpid();
	thr->pid = ff_getpid();
	id = thr->tid;
	ffly_ctl(ffly_malc, _ar_getpot, (ff_u64_t)&thr->pot);
	ffly_fprintf(ffly_out, "pid: %ld, tid: %lu\n", thr->pid, thr->tid);
	ffly_atomic_incr(&active_threads);
	ffly_cond_lock_signal(&thr->lock);
	thr->alive = 1;*/
//	ffly_ctl(ffly_malc, _ar_getpot, (ff_u64_t)&thr->pot);
//	ffly_fprintf(ffly_out, "pid: %ld, tid: %lu\n", thr->pid, thr->tid);
	thr->routine(thr->arg_p);
//	thr->alive = 0;
//	ffly_atomic_decr(&active_threads);

	thr->exit = 0;

	//ffly_thread_del(thr->tid);
//	ffly_mal_hang;
	exit(0);
}

ff_err_t ffly_thread_kill(ff_tid_t __tid) {
	struct ffly_thread *thr = get_thr(__tid);
	if (kill(thr->pid, SIGKILL) == -1) {
		ffly_fprintf(ffly_err, "thread, failed to kill..\n");
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}

void ffly_thread_wait(ff_tid_t __tid) {
	ffly_threadp t = get_thr(__tid);
	if (!t) {
		ffly_printf("error.\n");
		return;
	}
	ffly_printf("waiting for : %u\n", __tid);
	wait4(t->pid, NULL, __WALL|__WCLONE, NULL);
}

ff_err_t ffly_thread_create(ff_tid_t *__tid, void*(*__p)(void*), void *__arg_p) {
	if (no_threads == MAX_THREADS) {
		ffly_fprintf(ffly_log, "thread: only %u threads are allowed.\n", MAX_THREADS);
		return FFLY_FAILURE;
	}

	ffly_mutex_lock(&mutex);
	ff_u8_t reused_id = 0;
	if ((reused_id = (uu_ids.off>0))) {
		*__tid = *(uu_ids.p+(--uu_ids.off));
		if (uu_ids.off < ((uu_ids.page_c-1)*UU_PAGE_SIZE) && uu_ids.page_c > 1) {
			if ((uu_ids.p = (ff_tid_t*)__ffly_mem_realloc(uu_ids.p,
				((--uu_ids.page_c)*UU_PAGE_SIZE)*sizeof(ff_tid_t))) == NULL)
			{
				ffly_fprintf(ffly_err, "thread: failed to realloc memory space for unused thread ids.\n");
				return FFLY_FAILURE;
			}
		}
		ffly_mutex_unlock(&mutex);
		goto _exec;
	}

	*__tid = off++;
	ffly_printf("thread id: %u\n", off-1);

	ffly_mutex_unlock(&mutex);
	goto _alloc;

_exec:
	{
//	ffly_printf("stage, exec.\n");

	ffly_threadp thr = get_thr(*__tid);
	thr->alive = 0;
	thr->exit = -1;
	thr->lock = FFLY_COND_LOCK_INIT;
	if (reused_id) {
		thr->arg_p = __arg_p;
		thr->routine = __p;
	} else {
		*thr = (struct ffly_thread) {
			.tid = *__tid,
			.sp = (ff_byte_t*)__ffly_mem_alloc(DSS),
			.tls = (ff_byte_t*)__ffly_mem_alloc(TLS_SIZE),
			.arg_p = __arg_p,
			.routine = __p
		};
	}

	*(void**)(thr->sp+(DSS-8)) = (void*)prox;
	*(void**)(thr->sp+(DSS-16)) = (void*)thr;

	*(void**)thr->tls = thr->tls;
	__linux_pid_t pid;
	if ((pid = clone(CLONE_VM|CLONE_SIGHAND|CLONE_FILES|CLONE_FS|SIGCHLD|CLONE_SETTLS,
		(ff_u64_t)(thr->sp+(DSS-8)), NULL, NULL, (ff_u64_t)thr->tls)) == (__linux_pid_t)-1)
	{
		ffly_fprintf(ffly_err, "thread, failed to create.\n");
		return FFLY_FAILURE;
	}

	ffly_cond_lock_wait(&thr->lock);
	}
	return FFLY_SUCCESS;

_alloc:
//	ffly_printf("stage, alloc.\n");
	ffly_mutex_lock(&mutex);
	if (off > page_c*PAGE_SIZE) {
		if (!threads) {
			page_c++;
			if ((threads = (ffly_threadp*)__ffly_mem_alloc(PAGE_SIZE*sizeof(ffly_threadp))) == NULL) {
				ffly_fprintf(ffly_err, "thread: failed to alloc memory, errno{%d}, ffly_errno{%u}\n", 0, ffly_errno);
				goto _fail;
			}
		} else {
			if ((threads = (ffly_threadp*)__ffly_mem_realloc(threads,
				((++page_c)*PAGE_SIZE)*sizeof(ffly_threadp))) == NULL)
			{
				ffly_fprintf(ffly_err, "thread: failed to realloc memory, errno{%d}, ffly_errno{%u}\n", 0, ffly_errno);
				goto _fail;
			}
		}

		ffly_threadp *beg = threads+((page_c-1)*PAGE_SIZE);
		ffly_threadp *itr = beg, *end = beg+PAGE_SIZE;
		while(itr != end) // set all to null
			*(itr++) = NULL;
	}

	if (!*(threads+*__tid)) {
		*(threads+*__tid) = (ffly_threadp)__ffly_mem_alloc(sizeof(struct ffly_thread));
	}

	ffly_mutex_unlock(&mutex);
	goto _exec;
_fail:
	ffly_mutex_unlock(&mutex);
	return FFLY_FAILURE;
}

ff_err_t ffly_thread_cleanup() {
	while(active_threads != 0);
	ffly_threadp *itr = threads, cur;
	ffly_threadp *end = threads+off;
	ffly_fprintf(ffly_log, "thread shutdown, stage 0.\n");
	while(itr != end) {
		ffly_fprintf(ffly_log, "thread with id: %u, cleaning.\n", itr-threads);
		cur = *(itr++);
//		if (cur->alive)
//			ffly_thread_kill(cur->tid); // bad - rethink
		ffly_thread_wait(cur->tid);
		if (cur->sp != NULL) {
			ffly_fprintf(ffly_log, "\t.- stack release.\n");
			__ffly_mem_free(cur->sp);
		}
		if (cur->tls != NULL) {
			ffly_fprintf(ffly_log, "\t.- tls release.\n");
			__ffly_mem_free(cur->tls);
		}
		if (cur->exit == -1) { // if exit was a not a success and has been forced to stop
			ffly_ctl(ffly_malc, _ar_setpot, (ff_u64_t)cur->pot);
			ffly_arhang();	
			ffly_ctl(ffly_malc, _ar_unset, 0);
		}
		__ffly_mem_free(cur);
	}

	ffly_fprintf(ffly_log, "thread shutdown, stage 1.\n");
	if (threads != NULL) {
		__ffly_mem_free(threads);
		threads = NULL;
	}

	ffly_fprintf(ffly_log, "thread shutdown, stage 2.\n");
	if (uu_ids.p != NULL) {
		__ffly_mem_free(uu_ids.p);
		uu_ids.p = NULL;
	}
	uu_ids.off = uu_ids.page_c = 0;
	off = page_c = 0;
}
