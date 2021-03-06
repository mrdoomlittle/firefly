# ifndef __ffly__thread__h
# define __ffly__thread__h
# include "../ffint.h"
# include "../types.h"
# include "../system/errno.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../dep/mem_cpy.h"
# include "../dep/mem_dup.h"
# include "../linux/types.h"
# define FFLY_TID_NULL ((ff_tid_t)~0)
# define ffly_twait(__t) \
	ffly_thread_wait(__t)
# define ffly_tcreat(__t, __func, __arg) \
	ffly_thread_create(__t, __func, __arg)
# define ffly_tkill(__t) \
	ffly_thread_kill(__t)

# ifdef __cplusplus
extern "C" {
# endif
ff_tid_t ffly_gettid();
void ffly_thread_init();
ff_err_t ffly_thread_create(ff_tid_t*, void*(*)(void*), void*);
ff_err_t ffly_thread_kill(ff_tid_t);
ff_bool_t ffly_thread_alive(ff_tid_t);
ff_bool_t ffly_thread_dead(ff_tid_t);
void ffly_thread_wait(ff_tid_t);
__linux_pid_t ffly_thread_getpid(ff_tid_t);
ff_err_t ffly_thread_cleanup();
# ifdef __cplusplus
}
namespace mdl {
namespace firefly {
namespace system {
static ff_err_t(*thread_create)(ff_tid_t*, void*(*)(void*), void*) = &ffly_thread_create;

template<typename... _E> struct thread_arg;
template<typename _F, typename _A1>
struct thread_arg<_F, _A1> {
	_F f;
	_A1 a1;
};

template<typename _F, typename _A1, typename _A2>
struct thread_arg<_F, _A1, _A2> {
	_F f;
	_A1 a1;
	_A2 a2;
};

template<typename _F, typename _A1, typename _A2, typename _A3>
struct thread_arg<_F, _A1, _A2, _A3> {
	_F f;
	_A1 a1;
	_A2 a2;
	_A3 a3;
};

template<typename _F, typename _A1, typename _A2, typename _A3, typename _A4>
struct thread_arg<_F, _A1, _A2, _A3, _A4> {
	_F f;
	_A1 a1;
	_A2 a2;
	_A3 a3;
	_A4 a4;
};

template<typename _F>
static void* proxy(void *__arg) {
	(reinterpret_cast<_F>(__arg))();
	return nullptr;
}

template<typename _F, typename _A1>
static void* proxy(void *__arg) {
    struct thread_arg<_F, _A1> *arg = static_cast<struct thread_arg<_F, _A1>*>(__arg);
    arg->f(arg->a1);
	memory::mem_free(__arg);
	return nullptr;
}

template<typename _F, typename _A1, typename _A2>
static void* proxy(void *__arg) {
	struct thread_arg<_F, _A1, _A2> *arg = static_cast<struct thread_arg<_F, _A1, _A2>*>(__arg);
	arg->f(arg->a1, arg->a2);
	memory::mem_free(__arg);
	return nullptr;
}

template<typename _F, typename _A1, typename _A2, typename _A3>
static void* proxy(void *__arg) {
	struct thread_arg<_F, _A1, _A2, _A3> *arg = static_cast<struct thread_arg<_F, _A1, _A2, _A3>*>(__arg);
	arg->f(arg->a1, arg->a2, arg->a3);
	memory::mem_free(__arg);
	return nullptr;
}

template<typename _F, typename _A1, typename _A2, typename _A3, typename _A4>
static void* proxy(void *__arg) {
	struct thread_arg<_F, _A1, _A2, _A3, _A4> *arg = static_cast<struct thread_arg<_F, _A1, _A2, _A3, _A4>*>(__arg);
	arg->f(arg->a1, arg->a2, arg->a3, arg->a4);
	memory::mem_free(__arg);
	return nullptr;
}

struct thread {
	template<typename _F>
	thread(_F __f, ff_tid_t& __tid) {
		thread_create(&__tid, &proxy<_F>, (void*)__f);
	}

	template<typename _F, typename _A1>
	thread(_F __f, _A1 __a1, ff_tid_t& __tid) {
		struct thread_arg<_F, _A1> arg {
			f:__f,
			a1:__a1
		};

		void *_arg;
		ffly_mem_dupe(&_arg, (void*)&arg, sizeof(struct thread_arg<_F, _A1>));
		thread_create(&__tid, &proxy<_F, _A1>, _arg);
	}

	template<typename _F, typename _A1, typename _A2>
	thread(_F __f, _A1 __a1, _A2 __a2, ff_tid_t& __tid) {
		struct thread_arg<_F, _A1, _A2> arg {
			f:__f,
			a1:__a1,
			a2:__a2
		};

		void *_arg;
		ffly_mem_dupe(&_arg, (void*)&arg, sizeof(struct thread_arg<_F, _A1, _A2>));
		thread_create(&__tid, &proxy<_F, _A1, _A2>, _arg);
	}

	template<typename _F, typename _A1, typename _A2, typename _A3>
	thread(_F __f, _A1 __a1, _A2 __a2, _A3 __a3, ff_tid_t& __tid) {
		struct thread_arg<_F, _A1, _A2, _A3> arg {
			f:__f,
			a1:__a1,
			a2:__a2,
			a3:__a3
		};

		void *_arg;
		ffly_mem_dupe(&_arg, (void*)&arg, sizeof(struct thread_arg<_F, _A1, _A2, _A3>));
		thread_create(&__tid, &proxy<_F, _A1, _A2, _A3>, _arg);
	}

	template<typename _F, typename _A1, typename _A2, typename _A3, typename _A4>
	thread(_F __f, _A1 __a1, _A2 __a2, _A3 __a3, _A4 __a4, ff_tid_t& __tid) {
		struct thread_arg<_F, _A1, _A2, _A3, _A4> arg {
			f:__f,
			a1:__a1,
			a2:__a2,
			a3:__a3,
			a4:__a4
		};

		void *_arg;
		ffly_mem_dupe(&_arg, (void*)&arg, sizeof(struct thread_arg<_F, _A1, _A2, _A3, _A4>));
		thread_create(&__tid, &proxy<_F, _A1, _A2, _A3, _A4>, _arg);
	}
};
}
}
}
# endif
# endif /*__ffly__thread__h*/
