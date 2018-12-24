# include "thread.h"
# include "io.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_realloc.h"
# include "../memory/mem_free.h"
# include "../ctl.h"
# include "mutex.h"
# include "atomic.h"
# include "cond_lock.h"
# if !defined(__ffly_no_sysconf) && !defined(__ffly_crucial)
# define MAX_THREADS (*sysconf_get(max_threads))
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
# define T_USEABLE 0x1
# include "../log.h"

/*
	TODO:
		do somthing that does not involve {create and kill}
		have x amount of processes pending or use. 
*/
//# define debug
// no process linked
#define T_NOP 0x02
typedef struct ffly_thread {
	ff_cond_lock_t lock;
	__linux_pid_t pid;
	ff_u16_t h;
	ff_bool_t alive;
	ff_i8_t exit;
	ff_byte_t *sp, *tls;
	void*(*routine)(void*);
	void *arg_p;
	ffly_potp pot;
	ff_u8_t flags;
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
	ff_off_t *p;
	ff_uint_t page_c;
	ff_off_t off;
} uu = {
	.p = NULL,
	.page_c = 0,
	.off = 0
};

ff_mlock_t static mutex = FFLY_MUTEX_INIT;

/*
	find new name

	start from 0 to NHOLES
	and reloop is -1 then the hole is unused if >=0 then its used

	we could use timing but that can be left for higher level function to do
*/

#define NHOLES 48
ff_i32_t static holes[NHOLES] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

ff_i32_t static *nhole = holes;

ff_i32_t ffly_tobtain_hole(void) {
	if (nhole>=holes+NHOLES)
		nhole = holes;

	if (*nhole>=0) {
		return -1;
	}

	return (nhole++)-holes;
}

void ffly_trelinquish_hole(ff_u16_t __hole) {
	*(holes+__hole) = -1;
}

#define hset(__h, __val)\
	*(holes+(__h)) = (__val)
#define hget(__h)\
	*(holes+(__h))
#define get_thr(__t)\
	(*(threads+(__t)))
#define no_threads\
	(off-uu.off)

static ff_uint_t id;
ff_tid_t ffly_gettid() {
	return (ff_tid_t)ffly_tls_get(id);
}

__linux_pid_t ffly_thread_getpid(ff_tid_t __tid) {
	return get_thr(__tid)->pid;
}

void ffly_thread_init() {
	/* get current pot for this process
	*/
	ffly_ctl(ffly_malc, _ar_getpot, (ff_u64_t)&pot);
	id = ffly_tls_alloc(sizeof(ff_uint_t));
}

ff_err_t static
ffly_thread_del(ff_tid_t __t) {
	// keep memory on pot 0
	ffly_printf("goodbye from thread %u\n", __t);
	ffly_mutex_lock(&mutex);
	if (uu.off >= uu.page_c*UU_PAGE_SIZE) {
		ffly_ctl(ffly_malc, _ar_setpot, (ff_u64_t)pot);
		if (!uu.p) {
			if ((uu.p = (ff_off_t*)__ffly_mem_alloc(((++uu.page_c)*UU_PAGE_SIZE)*sizeof(ff_off_t))) == NULL) {
				ffly_fprintf(ffly_err, "thread: failed to allocate memory for unused thread ids to be stored.\n");
				goto _fail;
			}
		} else {
			if ((uu.p = (ff_off_t*)__ffly_mem_realloc(uu.p, ((++uu.page_c)*UU_PAGE_SIZE)*sizeof(ff_off_t))) == NULL) {
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
	*(uu.p+(uu.off++)) = hget(__t);
	hset(__t, -1);
	ffly_mutex_unlock(&mutex);
	return FFLY_SUCCESS;
}

// ??? 
void static
thread_free(ff_tid_t __t) {
	ff_i32_t h;
	if ((h = hget(__t)) == -1) {
		ffly_printf("no such thread.\n");
		return;
	}

	ffly_threadp thr;
	
	
	thr = get_thr(h);
	__ffly_mem_free(thr->sp);
	thr->sp = NULL;
}

ff_bool_t ffly_thread_alive(ff_tid_t __t) {
	ff_i32_t h;
	if ((h = hget(__t)) == -1) {
		return 0;
	}

	return get_thr(h)->alive;
}

ff_bool_t ffly_thread_dead(ff_tid_t __t) {
	ff_i32_t h;
	if ((h = hget(__t)) == -1) {
		return 0; // return errror
	}
	return !get_thr(h)->alive;
}

void static
prox(void) {
	void *arg_p;
	__asm__("movq -8(%%rbp), %%rdi\n\t"
			"movq %%rdi, %0": "=m"(arg_p) : : "rdi");
	ffly_threadp thr;

	ffly_tls_init();
	ffly_process_prep();
	thr = (ffly_threadp)arg_p;
	ffly_cond_lock_signal(&thr->lock);
# ifdef debug
	ffly_printf("new thread %u : %u.\n", thr->h, hget(thr->h));
# endif
	ffly_tls_set(thr->h, id);
	thr->routine(thr->arg_p);
	thr->exit = 0;

	ffly_thread_del(thr->h);
	ffly_mal_hang;
	thr->flags |= T_USEABLE;
	exit(0);
}

ff_err_t ffly_thread_kill(ff_tid_t __t) {
	ff_i32_t h;

	if ((h = hget(__t)) == -1) {
		ffly_printf("no such thread.\n");
		return FFLY_FAILURE;
	}

	ffly_threadp thr;
	
	thr = get_thr(h);
	if (kill(thr->pid, SIGKILL) == -1) {
		ffly_fprintf(ffly_err, "thread, failed to kill..\n");
		return FFLY_FAILURE;
	}
	return FFLY_SUCCESS;
}

void ffly_thread_wait(ff_tid_t __t) {
	ff_i32_t h;

	if ((h = hget(__t)) == -1) {
		ffly_printf("no such thread.\n");
		return;
	}

	ffly_threadp t;
	if (!(t = get_thr(h))) {
		ffly_printf("error.\n");
		return;
	}

	ffly_printf("waiting for : %u\n", h);
	wait4(t->pid, NULL, __WALL|__WCLONE, NULL);
}

ff_err_t ffly_thread_create(ff_tid_t *__tid, void*(*__p)(void*), void *__arg_p) {
	if (no_threads == MAX_THREADS) {
		ff_log("thread: only %u threads are allowed.\n", MAX_THREADS);
		return FFLY_FAILURE;
	}

	ffly_mutex_lock(&mutex);
	ff_i32_t h;
	if ((h = ffly_tobtain_hole()) == -1) {
		goto _fail;
	}

	ff_u8_t reused = 0;
	if ((reused = (uu.off>0))) {
		hset(*__tid = h, *(uu.p+(--uu.off)));
		if (uu.off < ((uu.page_c-1)*UU_PAGE_SIZE) && uu.page_c > 1) {
			if ((uu.p = (ff_off_t*)__ffly_mem_realloc(uu.p,
				((--uu.page_c)*UU_PAGE_SIZE)*sizeof(ff_off_t))) == NULL)
			{
				ffly_fprintf(ffly_err, "thread: failed to realloc memory space for unused thread ids.\n");
				return FFLY_FAILURE;
			}
		}
		ffly_mutex_unlock(&mutex);
		goto _exec;
	}

	hset(*__tid = h, off++);
	ffly_printf("thread id: %u\n", h);

	ffly_mutex_unlock(&mutex);
	goto _alloc;

_exec:
	{
//	ffly_printf("stage, exec.\n");

	ffly_threadp thr = get_thr(hget(h));

	if (!(thr->flags&T_NOP)) {
		/*
			wait for release
		*/
# ifdef debug 
		ffly_printf("waiting for thread %u to be released.\n", h);
# endif
		while(!(thr->flags&T_USEABLE));

		/*
			TODO:
				if the thread that we have here is on the edge of exiting
				pause it until be are done then tell it to jump to the start
		*/
		/*
			make sure thread is dead before use as the stack will be reused and 
			if we overwrite somthing important its going to cause havoc.
		*/
		wait4(thr->pid, NULL, __WALL|__WCLONE, NULL);
	}

# ifdef debug
	ffly_printf("thread was released.\n");
# endif
	if (reused) {
		thr->arg_p = __arg_p;
		thr->routine = __p;
	} else {
		*thr = (struct ffly_thread) {
			.sp = (ff_byte_t*)__ffly_mem_alloc(DSS),
			.tls = (ff_byte_t*)__ffly_mem_alloc(TLS_SIZE),
			.arg_p = __arg_p,
			.routine = __p
		};
	}

	thr->flags = 0x0;
	thr->alive = 0;
	thr->exit = -1;
	thr->lock = FFLY_COND_LOCK_INIT;
	thr->h = h;

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

	if (!*(threads+hget(h))) {
		ffly_threadp t;
		t = (*(threads+hget(h)) = (ffly_threadp)__ffly_mem_alloc(sizeof(struct ffly_thread)));
		t->flags = T_USEABLE|T_NOP;
	}

	ffly_mutex_unlock(&mutex);
	goto _exec;
_fail:
	ffly_mutex_unlock(&mutex);
	return FFLY_FAILURE;
}

ff_err_t ffly_thread_cleanup() {
	ffly_threadp *itr = threads, cur;
	ffly_threadp *end = threads+off;
	ff_log("thread/s - shutdown&cleanup, phase{0}.\n");
	while(itr != end) {
		ff_log("thread with id: %u, cleaning.\n", itr-threads);
		cur = *(itr++);

		wait4(cur->pid, NULL, __WALL|__WCLONE, NULL);

		if (cur->sp != NULL) {
			ff_log("\t.- stack release.\n");
			__ffly_mem_free(cur->sp);
		}
		if (cur->tls != NULL) {
			ff_log("\t.- tls release.\n");
			__ffly_mem_free(cur->tls);
		}
		if (cur->exit == -1) { // if exit was a not a success and has been forced to stop
			ffly_ctl(ffly_malc, _ar_setpot, (ff_u64_t)cur->pot);
			ffly_arhang();	
			ffly_ctl(ffly_malc, _ar_unset, 0);
		}
		__ffly_mem_free(cur);
		
	}

	ff_log("thread free, phase{1}.\n");
	if (threads != NULL) {
		__ffly_mem_free(threads);
		threads = NULL;
	}

	ff_log("uu free, phase{2}.\n");
	if (uu.p != NULL) {
		__ffly_mem_free(uu.p);
		uu.p = NULL;
	}
	uu.off = uu.page_c = 0;
	off = page_c = 0;
}
