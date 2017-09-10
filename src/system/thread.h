# ifndef __ffly__thread__h
# define __ffly__thread__h
# include "../types/err_t.h"
# include "../system/errno.h"
# include <stdio.h>
# include "../memory/mem_alloc.h"
# include "../data/mem_cpy.h"
# include "../data/mem_dupe.h"
# include "../types/thread.h"
# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_thread_create(ffly_tid_t*, void*(*)(void*), void*);
ffly_err_t ffly_thread_kill(ffly_tid_t);
ffly_err_t ffly_thread_cleanup();
# ifdef __cplusplus
}
namespace mdl {
namespace firefly {
namespace system {
static types::err_t(*thread_create)(ffly_tid_t*, void*(*)(void*), void*) = &ffly_thread_create;

template<typename _F, typename _A1, typename _A2, typename _A3, typename _A4>
struct thread_arg {
	_F f;
	_A1 a1;
	_A2 a2;
	_A3 a3;
	_A4 a4;
};

template<typename _F, typename _A1, typename _A2, typename _A3, typename _A4>
struct t {
static void* proxy(void *__arg) {
	struct thread_arg<_F, _A1, _A2, _A3, _A4> *arg = static_cast<struct thread_arg<_F, _A1, _A2, _A3, _A4>*>(__arg);
	arg->f(arg->a1, arg->a2, arg->a3, arg->a4);
}
};

struct thread {
	template<typename _F, typename _A1, typename _A2, typename _A3, typename _A4>
	thread(_F __f, _A1 __a1, _A2 __a2, _A3 __a3, _A4 __a4) {
		struct thread_arg<_F, _A1, _A2, _A3, _A4> arg {
			f:__f,
			a1:__a1,
			a2:__a2,
			a3:__a3,
			a4:__a4
		};

		void *_arg;
		ffly_tid_t tid;
		ffly_mem_dupe(&_arg, (void*)&arg, sizeof(struct thread_arg<_F, _A1, _A2, _A3, _A4>));
		thread_create(&tid, &t<_F, _A1, _A2, _A3, _A4>::proxy, _arg);
	}
};
}
}
}
# endif
# endif /*__ffly__thread__h*/
