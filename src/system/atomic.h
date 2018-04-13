# ifndef __ffly__atomic__h
# define __ffly__atomic__h
# include "../ffint.h"
# ifdef __cplusplus
extern "C" {
# endif
void ffly_atomic_add(ff_u64_t*, ff_u64_t);
void ffly_atomic_sub(ff_u64_t*, ff_u64_t);
void ffly_atomic_set(ff_u64_t*, ff_u64_t);
ff_u64_t ffly_atomic_get(ff_u64_t*);
void ffly_atomic_incr(ff_u64_t*);
void ffly_atomic_decr(ff_u64_t*);
# ifdef __cplusplus
}
namespace mdl {
namespace firefly {
namespace system {
typedef struct {
	ff::u64_t operator++(int) {
		ff::u64_t ret = this->d;
		ffly_atomic_incr(&this->d);
		return ret;
	}

	ff::u64_t operator--(int) {
		ff::u64_t ret = this->d;
		ffly_atomic_decr(&this->d);
		return ret;
	}

	ff::u64_t operator=(ff::u64_t __d) {
		this->d = __d;
		return __d;
	}

	ff::u64_t val() {return this->d;}
	ff::u64_t d;
} atomic_t;
}
}
}
# endif
# endif /*__ffly__atomic__h*/
