# ifndef __ffly__atomic__h
# define __ffly__atomic__h
# include <mdlint.h>
# ifdef __cplusplus
extern "C" {
# endif
void ffly_atomic_add(mdl_u64_t*, mdl_u64_t);
void ffly_atomic_sub(mdl_u64_t*, mdl_u64_t);
void ffly_atomic_set(mdl_u64_t*, mdl_u64_t);
mdl_u64_t ffly_atomic_get(mdl_u64_t*);
void ffly_atomic_incr(mdl_u64_t*);
void ffly_atomic_decr(mdl_u64_t*);
# ifdef __cplusplus
}
namespace mdl {
namespace firefly {
namespace system {
typedef struct {
	mdl_u64_t operator++(int) {
		mdl_u64_t ret = this->d;
		ffly_atomic_incr(&this->d);
		return ret;
	}

	mdl_u64_t operator--(int) {
		mdl_u64_t ret = this->d;
		ffly_atomic_decr(&this->d);
		return ret;
	}

	mdl_u64_t operator=(mdl_u64_t __d) {
		this->d = __d;
		return __d;
	}

	mdl_u64_t val() {return this->d;}
	mdl_u64_t d;
} atomic_t;
}
}
}
# endif

typedef mdl_u64_t ffly_atomic_u64_t;
typedef mdl_u64_t ffly_atomic_u32_t;
typedef mdl_u64_t ffly_atomic_u16_t;
typedef mdl_u64_t ffly_atomic_u8_t;
typedef mdl_u64_t ffly_atomic_uint_t;
# endif /*__ffly__atomic__h*/
