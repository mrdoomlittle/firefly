# ifndef __ffly_arr__h
# define __ffly_arr__h
# include <mdlint.h>
# include "../types/err_t.h"
# include "errno.h"
# include "../types/off_t.h"
struct ffly_arr {
	void *p;
	mdl_uint_t blk_c, blk_size;
};

# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_arr_init(struct ffly_arr*, mdl_uint_t, mdl_uint_t);
ffly_err_t ffly_arr_resize(struct ffly_arr*, mdl_uint_t);
ffly_err_t ffly_arr_de_init(struct ffly_arr*);
void* ffly_arr_get(struct ffly_arr*, ffly_off_t);
void ffly_arr_put(struct ffly_arr*, void*, ffly_off_t);
# ifdef __cplusplus
}

mdl_uint_t static __inline__ ffly_arr_blk_c(struct ffly_arr *__arr) {return __arr->blk_c;}

namespace mdl {
namespace firefly {
namespace system {
template<typename _T, mdl_uint_t _N>
struct arr {
	arr() {this->init();}
	~arr() {this->de_init();}
	types::err_t init() {
		ffly_arr_init(&this->raw_arr, sizeof(_T), _N);
	}

	types::err_t de_init() {
		ffly_arr_de_init(&this->raw_arr);
	}

	mdl_uint_t blk_c() {return ffly_arr_blk_c(&this->raw_arr);}

	_T get(types::off_t __blk_off){return *(_T*)ffly_arr_get(&this->raw_arr, __blk_off);}
	void put(_T __blk, types::off_t __blk_off){ffly_arr_put(&this->raw_arr, (void*)&__blk);}
	struct ffly_arr raw_arr;
};

}
}
}
# endif
# endif /*__ffly_arr__h*/
