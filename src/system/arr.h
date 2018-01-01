# ifndef __ffly_arr__h
# define __ffly_arr__h
# include <mdlint.h>
# include "../types/err_t.h"
# include "errno.h"
# include "../types/size_t.h"
# include "../types/off_t.h"
struct ffly_arr {
	void *p;
	mdl_uint_t blk_c;
	ffly_size_t blk_size;
};

typedef struct ffly_arr* ffly_arrp;
# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_arr_init(ffly_arrp, ffly_size_t, mdl_uint_t);
ffly_err_t ffly_arr_resize(ffly_arrp, mdl_uint_t);
ffly_err_t ffly_arr_de_init(ffly_arrp);
void* ffly_arr_get(ffly_arrp, ffly_off_t);
void ffly_arr_put(ffly_arrp, void*, ffly_off_t);
# ifdef __cplusplus
}
# endif
mdl_uint_t static __inline__ ffly_arr_blk_c(ffly_arrp __arr) {return __arr->blk_c;}
# ifdef __cplusplus
namespace mdl {
namespace firefly {
namespace system {
template<typename _T, mdl_uint_t _N>
struct arr {
	arr() {this->init();}
	~arr() {this->de_init();}
	types::err_t init() {
		ffly_arr_init(&this->raw, sizeof(_T), _N);
	}

	types::err_t de_init() {
		ffly_arr_de_init(&this->raw);
	}

	mdl_uint_t blk_c() {return ffly_arr_blk_c(&this->raw);}
	_T& at(types::off_t __off) {return *(_T*)ffly_arr_get(&this->raw, __off);}

	_T get(types::off_t __off){return *(_T*)ffly_arr_get(&this->raw, __off);}
	void put(_T __blk, types::off_t __off){ffly_arr_put(&this->raw, (void*)&__blk, __off);}
	struct ffly_arr raw;
};

}
}
}
# endif
# endif /*__ffly_arr__h*/
